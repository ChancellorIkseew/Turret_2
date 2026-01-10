#include "mobs_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/render/renderer.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/settings/settings.hpp"
#include "game/player/camera.hpp"
#include "mob_manager.hpp"

constexpr uint32_t HITBOX_COLOR = 0x5A'6D'75'A0;
constexpr uint32_t HEALTH_COLOR = 0xA5'23'23'FF;
constexpr PixelCoord BAR_SIZE(50.0f, 5.0f);

static t1_finline void move(MobSoA& soa, const size_t index, const PixelCoord vector) {
    soa.position[index] = soa.position[index] + vector;
    soa.hitbox[index].move(vector);
}

static inline void resolveCollisions(MobSoA& soa, const size_t current, const size_t mobCount, const Presets& presets) {
    for (size_t other = current + 1; other < mobCount; ++other) {
        if (!soa.hitbox[current].intersects(soa.hitbox[other])) continue;

        const PixelCoord overlap = soa.hitbox[current].overlap(soa.hitbox[other]);

        const bool pushX = (overlap.x < overlap.y);
        const float distance = pushX ? overlap.x : overlap.y;

        const float pushDirection = pushX ? (soa.position[current].x > soa.position[other].x ? 1.0f : -1.0f)
                                          : (soa.position[current].y > soa.position[other].y ? 1.0f : -1.0f);

        const MobPreset& currentPreset = presets.getMob(soa.preset[current]);
        const MobPreset& otherPreset   = presets.getMob(soa.preset[other]);

        const float totalRadius = currentPreset.hitboxRadius + otherPreset.hitboxRadius;
        const float invertedTotalRadius = 1.0f / totalRadius;
        const float mob1Weight = otherPreset.hitboxRadius   * invertedTotalRadius;
        const float mob2Weight = currentPreset.hitboxRadius * invertedTotalRadius;

        PixelCoord pushVec;
        if (pushX) pushVec.x = distance * pushDirection;
        else /*Y*/ pushVec.y = distance * pushDirection;

        move(soa, current, pushVec * mob1Weight);
        move(soa, other, pushVec * -mob2Weight);
    }
}

static inline void moveByAI(MobSoA& soa, const size_t mobCount) {
    for (size_t i = 0; i < mobCount; ++i) {
        move(soa, i, soa.velocity[i]);
    }
}

void mobs::processMobs(MobSoA& soa, const Presets& presets) {
    const size_t mobCount = soa.mobCount;
    moveByAI(soa, mobCount);
    for (size_t i = 0; i < mobCount; ++i) {
        resolveCollisions(soa, i, mobCount, presets);
    }
}

void mobs::cleanupMobs(MobManager& manager, const Presets& presets/*, Explosions& explosions*/) {
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
