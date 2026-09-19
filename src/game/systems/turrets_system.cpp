#include "turrets_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/audio/sound_queue.hpp"
#include "engine/render/renderer.hpp"
#include "engine/util/fast_rand.hpp"
#include "game/entities/mobs_pool.hpp"
#include "game/entities/particles_pool.hpp"
#include "game/entities/shells_pool.hpp"
#include "game/player/camera.hpp"
#include "turret_components.hpp"

constexpr PixelCoord NO_MOTION(0.0f, 0.0f);

static t1_finline_cxpr float normalize(const float angle) {
    if (angle >  t1::PI) return angle - 2.0f * t1::PI;
    if (angle < -t1::PI) return angle + 2.0f * t1::PI;
    return angle;
}

static t1_finline void rotateTurret(AngleRad& turretAngle, const AngleRad rotationSpeed, const PixelCoord facingVector) {
    const AngleRad requiredAngle = t1::atan(facingVector);
    const AngleRad deltaAngle = normalize(requiredAngle - turretAngle);
    //
    if (std::abs(deltaAngle) <= rotationSpeed)
        turretAngle = requiredAngle;
    else
        turretAngle += (deltaAngle > 0 ? rotationSpeed : -rotationSpeed);
}

static inline void rotateTurrets(TurretComponents& soa, const Presets& presets, const size_t mobCount) {
    if (soa.velocity.empty()) {
        for (size_t i = 0; i < mobCount; ++i) {
            const AngleRad rotationSpeed = presets.getTurret(soa.preset[i]).rotationSpeed;
            if (soa.shootingData[i].isShooting || soa.shootingData[i].aiType == ShootingAI::player_controlled)
                rotateTurret(soa.turretAngle[i], rotationSpeed, soa.shootingData[i].target - soa.position[i]);
        }
        return;
    }
    // else
    for (size_t i = 0; i < mobCount; ++i) {
        const AngleRad rotationSpeed = presets.getTurret(soa.preset[i]).rotationSpeed;
        if (soa.shootingData[i].isShooting)
            rotateTurret(soa.turretAngle[i], rotationSpeed, soa.shootingData[i].target - soa.position[i]);
        else if (soa.velocity[i] != NO_MOTION)
            rotateTurret(soa.turretAngle[i], rotationSpeed, soa.velocity[i]);
    }
}

static inline void reduceRestReload(TurretComponents& soa) {
    for (auto& time : soa.restReloadTime) {
        if (time > 0)
            --time;
    }
}

static inline void fallbackRecoil(TurretComponents& soa) {
    for (float& recoil : soa.currentRecoil) {
        constexpr float RECOIL_FALLBACK = 0.1f;
        recoil = std::max(0.f, recoil - RECOIL_FALLBACK);
    }
}

static inline void makeSparks(TurretComponents& soa, ParticlesPool& particles, const Presets& presets,
    const size_t mobCount, const Camera& camera, const uint64_t timeMs) {
    constexpr float SPARK_DENSITY = 0.05f;

    for (size_t i = 0; i < mobCount; ++i) {
        if (soa.restReloadTime[i] < 1 || !camera.contains(soa.position[i]))
            continue;

        const TurretPreset& turret = presets.getTurret(soa.preset[i]);
        if (turret.visual.sparkAreasCount < 1)
            continue;

        const PixelCoord sparkArea = turret.visual.sparkAreas[soa.currentBarrel[i]];
        const PixelCoord sparkAreaSize = turret.visual.sparkAreaSize;

        const float reloadProgress = static_cast<float>(soa.restReloadTime[i]) / static_cast<float>(turret.reload);
        const float area = sparkAreaSize.x * sparkAreaSize.y;
        const int sparksToSpawn = static_cast<int>(area * SPARK_DENSITY * t1::pow2f(reloadProgress));
        if (sparksToSpawn < 1)
            continue;

        const float sin = std::sin(soa.turretAngle[i]);
        const float cos = std::cos(soa.turretAngle[i]);
        const PixelCoord halfSize = sparkAreaSize * 0.5f;

        for (int j = 0; j < sparksToSpawn; ++j) {
            const float u = util::randMunus1to1(static_cast<uint32_t>(timeMs + i + j));
            const float v = util::randMunus1to1(static_cast<uint32_t>(timeMs * timeMs + i + j));

            const PixelCoord local(sparkArea.x + halfSize.x * u, sparkArea.y + halfSize.y * v - soa.currentRecoil[i]);

            PixelCoord position = soa.position[i];
            position.x +=  local.x * cos + local.y * sin;
            position.y += -local.x * sin + local.y * cos;

            constexpr PixelCoord SIZE(1, 1);
            constexpr TickCount LIFE_TIME = 5;
            constexpr uint32_t FADING = cl::fading(LIFE_TIME);
            particles.addParticle(position, SIZE, 0.f, 0.f, 0.f, 0.f, cl::CYAN, FADING, LIFE_TIME, PType::shard);
        }
    }
}

static inline void shoot(TurretComponents& soa, ShellsPool& shells, ParticlesPool& particles,
    const Presets& presets, const size_t mobCount, SoundQueue& sounds, const Camera& camera, const uint64_t timeMs) {
    for (size_t i = 0; i < mobCount; ++i) {
        if (soa.restReloadTime[i] > 0 || soa.ammo[i] < 1 || !soa.shootingData[i].isShooting)
            continue;
        --soa.ammo[i];
        const TurretPreset& turret = presets.getTurret(soa.preset[i]);
        const ShellPreset& shell = presets.getShell(turret.shell);
        soa.restReloadTime[i] = turret.reload;
        const AngleRad angle = soa.turretAngle[i] + util::randMunus1to1(static_cast<uint32_t>(timeMs + i)) * turret.spreadRad;
        const PixelCoord localMuzzle = turret.barrels[soa.currentBarrel[i]];
        PixelCoord position = soa.position[i];

        ++soa.currentBarrel[i];
        if (soa.currentBarrel[i] >= turret.barrelsCount)
            soa.currentBarrel[i] = 0;

        const float sin = sinf(angle);
        const float cos = cosf(angle);
        position.x +=  localMuzzle.x * cos + localMuzzle.y * sin;
        position.y += -localMuzzle.x * sin + localMuzzle.y * cos;

        PixelCoord velocity = PixelCoord(sin, cos) * presets.getShell(turret.shell).speed;
        if (!soa.velocity.empty())
            velocity += soa.velocity[i] * 0.8f;

        shells.addShell(turret.shell, position, velocity, angle, shell.damage, shell.maxLifeTime, soa.teamID[i]);
        if (camera.contains(position)) {
            position.x -= shell.visual.origin.y * sin;
            position.y -= shell.visual.origin.y * cos;
            constexpr PixelCoord SIZE(15.f, 15.f);
            particles.addParticle(position, SIZE, angle, 0.f, 0.2f, 0.f, cl::SMOKE, 0, 15, PType::smoke);
            particles.addParticle(position, SIZE / 1.5, angle, 0.f, 0.2f, 0.f, cl::ORANGE, 0, 15, PType::light);
            sounds.pushSound(turret.visual.shotSound, position);
            if (turret.visual.ejectionPortsCount > 0) {
                PixelCoord localPort = turret.visual.ejectionPorts[soa.currentBarrel[i]];
                localPort.y -= soa.currentRecoil[i];
                PixelCoord port = soa.position[i];
                port.x +=  localPort.x * cos + localPort.y * sin;
                port.y += -localPort.x * sin + localPort.y * cos;
                const PixelCoord size(shell.visual.size.y, shell.visual.size.x);
                const float casingAngle = localPort.x < 0 ? angle - t1::TAU + 0.2f : angle + t1::TAU - 0.2f;
                const float rotationSpeed = localPort.x < 0 ? -0.004f : 0.004f;
                constexpr uint32_t FADING = cl::fading(90);
                particles.addParticle(port, size, casingAngle, rotationSpeed, 0.2f, 0.002f, cl::BEIGE, FADING, 90, PType::shard);
            }
        }
        constexpr float MAX_RECOIL = 2.f;
        soa.currentRecoil[i] = std::min(MAX_RECOIL, soa.currentRecoil[i] + turret.recoil);
    }
}

void turrets::processTurrets(TurretComponents& soa, ShellsPool& shells, ParticlesPool& particles,
    const Presets& presets, SoundQueue& sounds, const Camera& camera, const uint64_t timeMs) {
    const size_t mobCount = soa.mobCount;
    reduceRestReload(soa);
    rotateTurrets(soa, presets, mobCount);
    shoot(soa, shells, particles, presets, mobCount, sounds, camera, timeMs);
    makeSparks(soa, particles, presets, mobCount, camera, timeMs);
    fallbackRecoil(soa);
}

void turrets::drawTurrets(TurretComponents&& soa, const Presets& presets, const Camera& camera, Renderer& renderer) {
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        if (!camera.contains(soa.position[i]))
            continue;
        const PixelCoord recoilVector(std::sin(soa.turretAngle[i]), std::cos(soa.turretAngle[i]));
        const PixelCoord position = soa.position[i] - recoilVector * soa.currentRecoil[i];
        const auto& visual = presets.getTurret(soa.preset[i]).visual;
        renderer.draw(visual.textureRect, position, visual.size, visual.origin, t1::PI - soa.turretAngle[i]);
    }
}

void turrets::drawShadows(TurretComponents&& soa, const Presets& presets, const Camera& camera, Renderer& renderer) {
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        if (!camera.contains(soa.position[i]))
            continue;
        const auto& visual = presets.getTurret(soa.preset[i]).visual;
        const PixelCoord shadowOffset(-visual.shadowOffset, visual.shadowOffset);
        const PixelCoord recoilVector(std::sin(soa.turretAngle[i]), std::cos(soa.turretAngle[i]));
        const PixelCoord position = soa.position[i] - recoilVector * soa.currentRecoil[i] + shadowOffset;
        renderer.draw(visual.textureRect, position, visual.size, visual.origin, t1::PI - soa.turretAngle[i], cl::SHADOW);
    }
}
