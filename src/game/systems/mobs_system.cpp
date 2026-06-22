#include "mobs_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/render/renderer.hpp"
#include "engine/coords/transforms.hpp"
#include "game/blocks/block_map.hpp"
#include "game/entities/chunk_grid.hpp"
#include "game/entities/mob_manager.hpp"
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

static inline void moveByAI(MobSoA& soa, const size_t mobCount) {
    for (size_t i = 0; i < mobCount; ++i) {
        soa.position[i] += soa.velocity[i];
    }
}

void mobs::processMobs(MobSoA& soa, const ChunkGrid& chunks, const BlockMap& blocks, const Presets& presets) {
    const size_t mobCount = soa.mobCount;
    moveByAI(soa, mobCount);
    resolveCollisions(soa, chunks, presets);
    resolveWorldCollisions(soa, mobCount, blocks, presets);
}

void mobs::cleanupMobs(MobManager& manager, const Presets& presets) {
    const auto& soa = manager.getSoa();
    // Reverse itaretion to avoid bugs with "swap and pop".
    for (size_t i = soa.mobCount; i > 0; --i) {
        size_t index = i - 1;
        if (soa.health[index] > 0)
            continue;
        // if (soa.presets->explosion.damage != 0)
        //     explosions.push(soa.presets->explosion);
        manager.removeMob(index);
    }
}

void mobs::drawHealthBars(const MobSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer) {
    constexpr uint32_t HITBOX_COLOR = 0x5A'6D'75'A0;
    constexpr uint32_t HEALTH_COLOR = 0xA5'23'23'FF;
    constexpr PixelCoord BAR_SIZE(50.0f, 5.0f);
    for (size_t i = 0; i < soa.id.size(); ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        const Health current = soa.health[i];
        const Health max = presets.getMob(soa.preset[i]).maxHealth;
        const float part = static_cast<float>(current) / static_cast<float>(max);
        const PixelCoord healthSize(BAR_SIZE.x * part, 5.0f);

        renderer.drawRect(soa.position[i] - BAR_SIZE / 2.0f, BAR_SIZE, {0.f, 0.f}, 0.f, HITBOX_COLOR);
        renderer.drawRect(soa.position[i] - BAR_SIZE / 2.0f, healthSize, {0.f, 0.f}, 0.f, HEALTH_COLOR);
    }
}

void mobs::drawMobs(MobSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer, const uint64_t tickCount) {
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        const auto& visual = presets.getMob(soa.preset[i]).visual;
        if (tickCount % visual.frameTicks == 0 && soa.velocity[i] != PixelCoord(0.0f, 0.0f)) {
            ++soa.chassisFrame[i];
            if (soa.chassisFrame[i] >= presets.getMob(soa.preset[i]).visual.frameCount)
                soa.chassisFrame[i] = 0;
        }

        TextureRect frameTextureRect = visual.textureRect;
        frameTextureRect.h = visual.frameHeight;
        frameTextureRect.y += static_cast<float>(visual.frameOrder[soa.chassisFrame[i]]) * visual.frameHeight;
        renderer.draw(frameTextureRect, soa.position[i], visual.size, visual.origin, t1::PI - soa.angle[i]);
    }
}

void mobs::drawMobShields(MobSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer, const uint64_t tickCount) {
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        if (soa.shieldHealth[i] < 1 || !camera.contains(t1::tile(soa.position[i])))
            continue;
        constexpr TextureRect RECT{ 0.f, 0.f, 1.f, 1.f };
        const auto& preset = presets.getMob(soa.preset[i]);
        PixelCoord origin(preset.shieldRadius, preset.shieldRadius);
        PixelCoord size = origin * 2.f;
        renderer.draw(RECT, soa.position[i], size, origin, 0.f, 0xFF'FF'FF'00);
    }
}
