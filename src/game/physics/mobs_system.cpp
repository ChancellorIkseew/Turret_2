#include "mobs_system.hpp"
//
#include <algorithm>
#include "engine/assets/presets.hpp"
#include "engine/render/renderer.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/settings/settings.hpp"
#include "game/blocks/blocks.hpp"
#include "game/physics/chunk_grid.hpp"
#include "game/player/camera.hpp"
#include "game/player/player_controller.hpp"
#include "mob_manager.hpp"

constexpr uint32_t HITBOX_COLOR = 0x5A'6D'75'A0;
constexpr uint32_t HEALTH_COLOR = 0xA5'23'23'FF;
constexpr PixelCoord BAR_SIZE(50.0f, 5.0f);

static inline void resolveCollision(MobSoA& soa, const size_t current, const size_t other) {
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

static inline void resolveCollisions(MobSoA& soa, const ChunkGrid& chunks) {
    for (const Chunk chunk : chunks.getPopulatedChunks()) {
        for (const auto* currentMob = chunk.begin(); currentMob < chunk.end(); ++currentMob) {
            for (const auto* otherMob = currentMob + 1; otherMob < chunk.end(); ++otherMob) {
                resolveCollision(soa, *currentMob, *otherMob);
            }
        }
    }
}

static inline void resolveWorldCollisions(MobSoA& soa, const size_t mobCount, const Blocks& blocks) {
    for (size_t i = 0; i < mobCount; ++i) {
        const float radius = soa.hitboxRadius[i];
        const PixelCoord center = soa.position[i];

        const TileCoord start = t1::tile(center - PixelCoord(radius, radius));
        const TileCoord end = t1::tile(center + PixelCoord(radius, radius));

        for (int32_t x = start.x; x <= end.x; ++x) {
            for (int32_t y = start.y; y <= end.y; ++y) {
                const TileCoord tile{ x,y };
                if (!blocks.isFilledBlock(tile))
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

void mobs::processMobs(MobSoA& soa, const ChunkGrid& chunks, const Blocks& blocks) {
    const size_t mobCount = soa.mobCount;
    moveByAI(soa, mobCount);
    resolveCollisions(soa, chunks);
    resolveWorldCollisions(soa, mobCount, blocks);
}

void mobs::cleanupMobs(MobManager& manager, const Presets& presets, PlayerController& plCtr) {
    const auto& soa = manager.getSoa();
    // Reverse itaretion to avoid bugs with "swap and pop".
    for (size_t i = soa.mobCount; i > 0; --i) {
        size_t index = i - 1;
        if (soa.health[index] > 0)
            continue;
        // if (soa.presets->explosion.damage != 0)
        //     explosions.push(soa.presets->explosion);
        if (plCtr.getTarget() == soa.id[index])
            plCtr.resetTarget(manager, presets);
        manager.removeMob(index);
    }
}

static void drawHitboxes(const MobSoA& soa, const Presets& presets, const Camera& camera, const Renderer& renderer) {
    for (size_t i = 0; i < soa.id.size(); ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        const float hitboxSize = presets.getMob(soa.preset[i]).hitboxRadius * 2.0f;
        const PixelCoord hitbox(hitboxSize, hitboxSize);
        renderer.drawRect(HITBOX_COLOR, soa.position[i] - hitbox / 2.0f, hitbox);
    }
}

static void drawHealthBars(const MobSoA& soa, const Presets& presets, const Camera& camera, const Renderer& renderer) {
    for (size_t i = 0; i < soa.id.size(); ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        const Health current = soa.health[i];
        const Health max = presets.getMob(soa.preset[i]).maxHealth;
        const float part = static_cast<float>(current) / static_cast<float>(max);
        const PixelCoord healthSize(BAR_SIZE.x * part, 5.0f);

        renderer.drawRect(HITBOX_COLOR, soa.position[i] - BAR_SIZE / 2.0f, BAR_SIZE);
        renderer.drawRect(HEALTH_COLOR, soa.position[i] - BAR_SIZE / 2.0f, healthSize);
    }
}

void mobs::drawMobs(MobSoA& soa, const Presets& presets, const Camera& camera, const Renderer& renderer, const uint64_t tickCount) {
    if (Settings::gameplay.showHitboxes)
        drawHitboxes(soa, presets, camera, renderer);
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        auto& visual = presets.getMob(soa.preset[i]).visual;
        if (tickCount % visual.frameTicks == 0 && soa.velocity[i] != PixelCoord(0.0f, 0.0f)) {
            ++soa.chassisFrame[i];
            if (soa.chassisFrame[i] >= presets.getMob(soa.preset[i]).visual.frameCount)
                soa.chassisFrame[i] = 0;
        }
        renderer.drawAnimated(visual.texture, soa.position[i], visual.size, visual.origin, soa.angle[i],
            visual.frameOrder[soa.chassisFrame[i]], visual.frameHeight);
    }
}
