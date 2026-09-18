#include "mobs_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/render/renderer.hpp"
#include "game/blocks/block_map.hpp"
#include "game/entities/chunk_grid.hpp"
#include "game/entities/mobs_pool.hpp"
#include "game/entities/particles_pool.hpp"
#include "game/player/camera.hpp"

static inline void resolveCollision(MobSoA& soa, const size_t current, const size_t other, const Presets& presets) {
    const bool currentFlying = presets.getMob(soa.preset[current]).flying;
    const bool otherFlying   = presets.getMob(soa.preset[other]).flying;
    if (currentFlying != otherFlying) // both ground or both flying
        return;

    const CircleHitbox currentHitbox(soa.position[current], soa.hitboxRadius[current]);
    const CircleHitbox otherHitbox(soa.position[other], soa.hitboxRadius[other]);
    if (!currentHitbox.intersects(otherHitbox))
        return;

    const PixelCoord overlap = currentHitbox.overlap(otherHitbox);

    const float currentMobMass = t1::pow2f(soa.hitboxRadius[current]);
    const float otherMobMass   = t1::pow2f(soa.hitboxRadius[other]);
    const float totalMass = currentMobMass + otherMobMass;
    const float invertedTotalMass = 1.0f / totalMass;
    const float currentMobWeight = currentMobMass * invertedTotalMass;
    const float otherMobWeight   = otherMobMass   * invertedTotalMass;

    soa.position[current] += overlap * otherMobWeight;
    soa.position[other]   -= overlap * currentMobWeight;
}

static inline void resolveWorldCollision(MobSoA& soa, const size_t mob, const RectHitbox& block) {
    const CircleHitbox mobHitbox(soa.position[mob], soa.hitboxRadius[mob]);
    const PixelCoord overlap = mobHitbox.overlap(block);
    if (overlap != PixelCoord(0.0f, 0.0f))
        soa.position[mob] += overlap;
}

static inline void resolveCollisions(MobSoA& soa, const ChunkGrid& chunks, const Presets& presets) {
    for (const Chunk chunk : chunks.getPopulatedChunks()) {
        for (const auto* currentMob = chunk.begin(); currentMob < chunk.end(); ++currentMob) {
            for (const auto* otherMob = currentMob + 1; otherMob < chunk.end(); ++otherMob) {
                resolveCollision(soa, *currentMob, *otherMob, presets);
            }
        }
    }
}

static inline void resolveWorldCollisions(MobSoA& soa, const size_t mobCount, const BlockMap& blocks, const Presets& presets) {
    for (size_t i = 0; i < mobCount; ++i) {
        if (presets.getMob(soa.preset[i]).flying) // no world collisions for flying mobs
            continue;
        const float radius = soa.hitboxRadius[i];
        const PixelCoord center = soa.position[i];

        const TileCoord start = t1::tile(center - PixelCoord(radius, radius));
        const TileCoord end = t1::tile(center + PixelCoord(radius, radius));

        for (int32_t x = start.x; x <= end.x; ++x) {
            for (int32_t y = start.y; y <= end.y; ++y) {
                const TileCoord tile{ x,y };
                if (!blocks.isFilled(tile))
                    continue;
                const RectHitbox tileHitbox{ t1::pixel(tile), t1::pixel(tile) + t1::TILE_PC };
                resolveWorldCollision(soa, i, tileHitbox);
            }
        }
    }
}

static inline void moveByVelocity(MobSoA& soa, const size_t mobCount) {
    for (size_t i = 0; i < mobCount; ++i) {
        soa.preveousePosition[i] = soa.position[i];
        soa.position[i] += soa.velocity[i];
    }
}

static inline void animateMoving(MobSoA& soa, const size_t mobCount, const Presets& presets) {
    for (size_t i = 0; i < mobCount; ++i) {
        if (t1::areCloserCircle(soa.position[i], soa.preveousePosition[i], 0.04f))
            continue;
        const auto& visual = presets.getMob(soa.preset[i]).visual;
        ++soa.chassisTick[i];
        const uint8_t frame = soa.chassisTick[i] / visual.frameTicks;
        if (frame >= visual.frameCount)
            soa.chassisTick[i] = 0;
    }
}

static inline void makeTrails(const MobSoA& soa, ParticlesPool& particlesPool, const Presets& presets,
    const Camera& camera, const size_t mobCount) {
    for (size_t i = 0; i < mobCount; ++i) {
        constexpr PixelCoord SIZE(5, 5);
        constexpr uint32_t FADING = uint32_t(float(0xFF) / 8.f);
        const auto& preset = presets.getMob(soa.preset[i]);
        const auto& visual = preset.visual;

        if (!camera.contains(soa.position[i]) || !preset.flying)
            continue;
        for (size_t e = 0; e < visual.enginesCount; ++e) {
            PixelCoord position = soa.position[i];
            const PixelCoord localMuzzle = visual.engines[e];

            const float sin = sinf(soa.angle[i]);
            const float cos = cosf(soa.angle[i]);

            position.x += localMuzzle.x * cos + localMuzzle.y * sin;
            position.y += -localMuzzle.x * sin + localMuzzle.y * cos;

            const TickCount lifeTime = t1::areCloserCircle(soa.position[i], soa.preveousePosition[i], 0.04f) ? 2 : 8;
            particlesPool.addParticle(position, SIZE * 1.5f, 0.f, 0.f, 0.f, 0.f, 0x80'80'80'FF, 0, 2, PType::smoke);
            particlesPool.addParticle(position, SIZE, 0.f, 0.f, 0.f, 0.f, cl::ORANGE, FADING, lifeTime, PType::light);
            particlesPool.addParticle(position, SIZE * 1.5f, 0.f, 0.f, 0.f, 0.f, 0x00'60'60'FF, 0, 2, PType::light);
        }
    }
}

void mobs::processMobs(MobSoA& soa, const ChunkGrid& chunks, const BlockMap& blocks, const Presets& presets,
    ParticlesPool& particles, const Camera& camera) {
    const size_t mobCount = soa.mobCount;
    moveByVelocity(soa, mobCount);
    resolveCollisions(soa, chunks, presets);
    resolveWorldCollisions(soa, mobCount, blocks, presets);
    animateMoving(soa, mobCount, presets);
    makeTrails(soa, particles, presets, camera, mobCount);
    for (auto& ammo : soa.ammo) {
        ammo = 1;
    }
}

void mobs::cleanupMobs(MobsPool& mobsPool, const Presets& presets) {
    const auto& soa = mobsPool.getSoa();
    // Reverse itaretion to avoid bugs with "swap and pop".
    for (size_t i = soa.mobCount; i > 0; --i) {
        size_t index = i - 1;
        if (soa.health[index] > 0)
            continue;
        // if (soa.presets->explosion.damage != 0)
        //     explosions.push(soa.presets->explosion);
        mobsPool.removeMob(index);
    }
}

void mobs::drawHealthBars(const MobSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer) {
    constexpr uint32_t HITBOX_COLOR = 0x5A'6D'75'A0;
    constexpr uint32_t HEALTH_COLOR = 0xA5'23'23'FF;
    constexpr PixelCoord BAR_SIZE(50.0f, 5.0f);
    for (size_t i = 0; i < soa.mobCount; ++i) {
        if (!camera.contains(soa.position[i]))
            continue;
        const Health current = soa.health[i];
        const Health max = presets.getMob(soa.preset[i]).maxHealth;
        const float part = static_cast<float>(current) / static_cast<float>(max);
        const PixelCoord healthSize(BAR_SIZE.x * part, 5.0f);

        renderer.drawRect(soa.position[i] - BAR_SIZE / 2.0f, BAR_SIZE, {0.f, 0.f}, 0.f, HITBOX_COLOR);
        renderer.drawRect(soa.position[i] - BAR_SIZE / 2.0f, healthSize, {0.f, 0.f}, 0.f, HEALTH_COLOR);
    }
}

void mobs::drawMobs(MobSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer) {
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        if (!camera.contains(soa.position[i]))
            continue;
        const auto& visual = presets.getMob(soa.preset[i]).visual;
        const uint8_t frame = soa.chassisTick[i] / visual.frameTicks;
        TextureRect frameTextureRect = visual.textureRect;
        frameTextureRect.h = visual.frameHeight;
        frameTextureRect.y += static_cast<float>(visual.frameOrder[frame]) * visual.frameHeight;
        renderer.draw(frameTextureRect, soa.position[i], visual.size, visual.origin, t1::PI - soa.angle[i]);
    }
}

void mobs::drawMobShields(const MobSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer, const uint64_t tickCount) {
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        if (soa.shieldHealth[i] < 1 || !camera.contains(soa.position[i]))
            continue;
        const auto& preset = presets.getMob(soa.preset[i]);
        const PixelCoord origin(preset.shieldRadius, preset.shieldRadius);
        const PixelCoord size = origin * 2.f;
        renderer.draw(FULL_UV_RECT, soa.position[i], size, origin);
    }
}

static std::optional<size_t> findPlayerControlled(const std::vector<ShootingData>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i].aiType == ShootingAI::player_controlled)
            return i;
    }
    return std::nullopt;
}

void mobs::drawEnemyMarkers(const TeamID playerTeamID, const MobSoA& soa, const Camera& camera, Renderer& renderer) {
    const size_t mobCount = soa.mobCount;
    const auto optPlayerMob = findPlayerControlled(soa.shootingData);
    const PixelCoord cameraCenter = optPlayerMob ? soa.position[optPlayerMob.value()] : camera.getRealCenter();
    const PixelCoord windowCenter = camera.fromMapToScreen(cameraCenter);

    for (size_t i = 0; i < mobCount; ++i) {
        if (soa.teamID[i] == playerTeamID || t1::areCloserCircle(soa.position[i], cameraCenter, 100.f))
            continue;
        constexpr PixelCoord MARKER_SIZE(5.f, 20.f);
        constexpr PixelCoord MARKER_ORIGIN(2.5f, -100.f);
        float angle = t1::atan(cameraCenter - soa.position[i]);
        renderer.drawRect(windowCenter, MARKER_SIZE, MARKER_ORIGIN, t1::PI - angle, 0x84'34'34'FF);
    }
}

void mobs::drawFlyingMobs(MobSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer) {
    const size_t mobCount = soa.mobCount;

    for (size_t i = 0; i < mobCount; ++i) {
        if (!camera.contains(soa.position[i]))
            continue;
        const auto& preset = presets.getMob(soa.preset[i]);
        if (!preset.flying)
            continue;
        const auto& visual = preset.visual;

        constexpr PixelCoord SHADOW_OFFSET(-20, 20);
        renderer.draw(visual.textureRect, soa.position[i] + SHADOW_OFFSET, visual.size, visual.origin, t1::PI - soa.angle[i], cl::SHADOW);
    }

    for (size_t i = 0; i < mobCount; ++i) {
        if (!camera.contains(soa.position[i]))
            continue;
        const auto& preset = presets.getMob(soa.preset[i]);
        if (!preset.flying)
            continue;
        const auto& visual = preset.visual;
        const uint8_t frame = soa.chassisTick[i] / visual.frameTicks;
        TextureRect frameTextureRect = visual.textureRect;
        frameTextureRect.h = visual.frameHeight;
        frameTextureRect.y += static_cast<float>(visual.frameOrder[frame]) * visual.frameHeight;
        renderer.draw(frameTextureRect, soa.position[i], visual.size, visual.origin, t1::PI - soa.angle[i]);
    }

    for (size_t i = 0; i < mobCount; ++i) {
        if (!camera.contains(soa.position[i]))
            continue;
        const auto& preset = presets.getMob(soa.preset[i]);
        if (!preset.flying)
            continue;
        const PixelCoord recoilVector(std::sin(soa.turretAngle[i]), std::cos(soa.turretAngle[i]));
        const PixelCoord position = soa.position[i] - recoilVector * soa.currentRecoil[i];
        const auto& visual = presets.getTurret(preset.turret).visual;
        renderer.draw(visual.textureRect, position, visual.size, visual.origin, t1::PI - soa.turretAngle[i]);
    }
}
