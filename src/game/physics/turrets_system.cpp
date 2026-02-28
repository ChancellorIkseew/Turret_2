#include "turrets_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/coords/math.hpp"
#include "engine/audio/sound_queue.hpp"
#include "engine/render/renderer.hpp"
#include "game/particles/particle_manager.hpp"
#include "game/player/camera.hpp"
#include "mob_manager.hpp"
#include "shell_manager.hpp"
#include "turret_components.hpp"

constexpr PixelCoord NO_MOTION(0.0f, 0.0f);

static t1_finline_cxpr float normalize(const float angle) {
    if (angle >  t1::PI_F) return angle - 2.0f * t1::PI_F;
    if (angle < -t1::PI_F) return angle + 2.0f * t1::PI_F;
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
            if (soa.shootingData[i].isShooting)
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

static inline void shoot(TurretComponents& soa, ShellManager& shells, ParticleManager& particles,
    const Presets& presets, const size_t mobCount, SoundQueue& sounds, const Camera& camera) {
    for (size_t i = 0; i < mobCount; ++i) {
        if (soa.restReloadTime[i] > 0 || !soa.shootingData[i].isShooting)
            continue;
        const TurretPreset& turret = presets.getTurret(soa.preset[i]);
        const ShellPreset& shell = presets.getShell(turret.shell);
        soa.restReloadTime[i] = turret.reload;
        const AngleRad angle = soa.turretAngle[i];
        const PixelCoord localMuzzle = turret.barrels[soa.currentBarrel[i]];
        PixelCoord position = soa.position[i];

        ++soa.currentBarrel[i];
        if (soa.currentBarrel[i] >= turret.barrelsCount)
            soa.currentBarrel[i] = 0;

        const float sin = sinf(angle);
        const float cos = cosf(angle);
        position.x +=  localMuzzle.x * cos + localMuzzle.y * sin;
        position.y += -localMuzzle.x * sin + localMuzzle.y * cos;

        shells.addShell(presets, turret.shell, position, angle, shell.damage, shell.maxLifeTime, soa.teamID[i]);
        if (camera.contains(t1::tile(position))) {
            position.x -= shell.visual.origin.y * sin;
            position.y -= shell.visual.origin.y * cos;
            particles.addParticle(position, angle, 0.2f, 0xFF'B3'69'A0, 15);
            sounds.pushSound(turret.visual.shotSound, position);
        }  
    }
}

void turrets::processTurrets(TurretComponents& soa, ShellManager& shells, ParticleManager& particles,
    const Presets& presets, SoundQueue& sounds, const Camera& camera) {
    const size_t mobCount = soa.mobCount;
    reduceRestReload(soa);
    rotateTurrets(soa, presets, mobCount);
    shoot(soa, shells, particles, presets, mobCount, sounds, camera);
}

void turrets::drawTurrets(TurretComponents&& soa, const Presets& presets, const Camera& camera, const Renderer& renderer) {
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        auto& visual = presets.getTurret(soa.preset[i]).visual;
        renderer.draw(visual.texture, soa.position[i], visual.size, visual.origin, soa.turretAngle[i]);
    }
}
