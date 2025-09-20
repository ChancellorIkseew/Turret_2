#include "mobs_system.hpp"
//
#include <mutex>
#include "engine/coords/transforms.hpp"
#include "engine/settings/settings.hpp"
#include "game/world/camera.hpp"
#include "team/teams_pool.hpp"

static std::mutex mobsMutex;
static Sprite hitboxSprite;

static t1_finline void move(Mob& mob, const PixelCoord vector) {
    mob.position = mob.position + vector;
    mob.hitbox.move(vector);
}

static t1_finline void moveX(Mob& mob, const float x) {
    mob.position.x = mob.position.x + x;
    mob.hitbox.move(PixelCoord(x, 0.0f));
}

static t1_finline void moveY(Mob& mob, const float y) {
    mob.position.y = mob.position.y + y;
    mob.hitbox.move(PixelCoord(0.0f, y));
}

static inline void resolveCollisions(Mob& mob, std::list<Mob>& mobs) {
    for (auto& otherMob : mobs) {
        if (&mob == &otherMob || !mob.hitbox.intersects(otherMob.hitbox))
            continue;
        
        PixelCoord overlap = mob.hitbox.overlap(otherMob.hitbox);
        const float w = 1.0f / (mob.preset.hitboxRadius + otherMob.preset.hitboxRadius);
        if (overlap.x > overlap.y) {
            if (mob.position.y > otherMob.position.y) {
                moveY(mob,             w * overlap.y * otherMob.preset.hitboxRadius);
                moveY(otherMob, -1.f * w * overlap.y * mob.preset.hitboxRadius);   
            }
            else {
                moveY(mob,      -1.f * w * overlap.y * otherMob.preset.hitboxRadius);
                moveY(otherMob,        w * overlap.y * mob.preset.hitboxRadius);
            }
        }
        else {
            if (mob.position.x > otherMob.position.x) {
                moveX(mob,             w * overlap.x * otherMob.preset.hitboxRadius);
                moveX(otherMob, -1.f * w * overlap.x * mob.preset.hitboxRadius);
            }
            else {
                moveX(mob,      -1.f * w * overlap.x * otherMob.preset.hitboxRadius);
                moveX(otherMob,        w * overlap.x * mob.preset.hitboxRadius);
            }
        }
    }
}

void mobs::processMobs(std::list<Mob>& mobs, TeamsPool& teams) {
    for (auto& mob : mobs) {
        resolveCollisions(mob, mobs);
        if (!mob.movingAI)
            continue;
        mob.movingAI->update(mob);
        mob.velocity = mob.movingAI->getMotionVector() * mob.preset.speed;
        mob.angle = mob.movingAI->getMotionAngle();
        move(mob, mob.velocity);
    }
    //
    std::lock_guard<std::mutex> guard(mobsMutex);
    mobs.remove_if([](const Mob& mob) { return mob.wasted; });
}

static void drawHitboxes(const std::list<Mob>& mobs, const Camera& camera) {
    for (auto& mob : mobs) {
        if (!camera.contains(t1::tile(mob.position)))
            continue;
        // TODO: refactoring
        const float hitboxSize = mob.preset.hitboxRadius * 2.0f;
        const PixelCoord hitbox(hitboxSize, hitboxSize);
        hitboxSprite.setTexture(Texture("fill"));
        hitboxSprite.setSize(hitbox);
        hitboxSprite.setOrigin(hitbox / 2.0f);
        hitboxSprite.setPosition(mob.position);
        hitboxSprite.draw();
    }
}

void mobs::drawMobs(std::list<Mob>& mobs, const Camera& camera) {
    std::lock_guard<std::mutex> guard(mobsMutex);
    if (Settings::gameplay.showHitboxes)
        drawHitboxes(mobs, camera);
    for (auto& mob : mobs) {
        if (!camera.contains(t1::tile(mob.position)))
            continue;
        mob.sprite.setPosition(mob.position);
        mob.sprite.setRotationRad(mob.angle);
        mob.sprite.draw();
    }
}
