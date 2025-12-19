#include "mobs_system.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/settings/settings.hpp"
#include "game/player/camera.hpp"
#include "mob_manager.hpp"

static t1_finline void move(MobSoA& soa, const size_t index, const PixelCoord vector) {
    soa.position[index] = soa.position[index] + vector;
    soa.hitbox[index].move(vector);
}

static inline void resolveCollisions(MobSoA& soa, const size_t current, const size_t end) {
    for (size_t other = current + 1; other < end; ++other) {
        if (!soa.hitbox[current].intersects(soa.hitbox[other])) continue;

        const PixelCoord overlap = soa.hitbox[current].overlap(soa.hitbox[other]);

        const bool pushX = (overlap.x < overlap.y);
        const float distance = pushX ? overlap.x : overlap.y;

        const float pushDirection = pushX ? (soa.position[current].x > soa.position[other].x ? 1.0f : -1.0f)
            : (soa.position[current].y > soa.position[other].y ? 1.0f : -1.0f);

        const float totalRadius = soa.preset[current]->hitboxRadius + soa.preset[other]->hitboxRadius;
        const float invertedTotalRadius = 1.0f / totalRadius;
        const float mob1Weight = soa.preset[other]->hitboxRadius * invertedTotalRadius;
        const float mob2Weight = soa.preset[current]->hitboxRadius * invertedTotalRadius;

        PixelCoord pushVec;
        if (pushX) pushVec.x = distance * pushDirection;
        else /*Y*/ pushVec.y = distance * pushDirection;

        move(soa, current, pushVec * mob1Weight);
        move(soa, other, pushVec * -mob2Weight);
    }
}

static inline void moveByAI(MobSoA& soa, const size_t index) {
    if (false)
        return;
    //updateAI
    PixelCoord AIVector; //getFromAI
    Angle AIAngle = 0; //getFromAI
    soa.velocity[index] = AIVector * soa.preset[index]->maxSpeed;
    soa.angle[index] = AIAngle;
    move(soa, index, soa.velocity[index]);
}

void mobs::processMobs(MobSoA& soa) {
    size_t end = soa.id.size();
    for (size_t i = 0; i < end; ++i) {
        resolveCollisions(soa, i, end);
    }
}

static void drawHitboxes(const MobSoA& soa, const Camera& camera) {
    Sprite sprite;
    for (size_t i = 0; i < soa.id.size(); ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        // TODO: refactoring
        const float hitboxSize = soa.preset[i]->hitboxRadius * 2.0f;
        const PixelCoord hitbox(hitboxSize, hitboxSize);
        sprite.setTexture(Texture("fill"));
        sprite.setSize(hitbox);
        sprite.setOrigin(hitbox / 2.0f);
        sprite.setPosition(soa.position[i]);
        sprite.draw();
    }
}

void mobs::drawMobs(const MobSoA& soa, const Camera& camera, const float tickOfset) {
    if (Settings::gameplay.showHitboxes)
        drawHitboxes(soa, camera);
    Sprite sprite;
    for (size_t i = 0; i < soa.id.size(); ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        auto& visual = soa.preset[i]->visual;
        sprite.setTexture(*visual.texture);
        sprite.setOrigin(visual.origin);
        sprite.setSize(visual.size);
        sprite.setPosition(soa.position[i] + soa.velocity[i] * tickOfset);
        sprite.setRotationRad(soa.angle[i]);
        sprite.draw();
    }
}
