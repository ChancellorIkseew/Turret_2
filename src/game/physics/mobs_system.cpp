#include "mobs_system.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/settings/settings.hpp"
#include "game/player/camera.hpp"
#include "team/teams_pool.hpp"

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
        
        mob.colided = true;
        otherMob.colided = true;
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
        mob.colided = false; //TODO: find better way to resolve collisions.
    }
    for (auto& mob : mobs) {
        resolveCollisions(mob, mobs);
        if (!mob.movingAI)
            continue;
        mob.movingAI->update(mob);
        mob.velocity = mob.movingAI->getMotionVector() * mob.preset.speed;
        mob.angle = mob.movingAI->getMotionAngle();
        move(mob, mob.velocity);
    }
    for (auto& mob : mobs) {
        if (!mob.turret)
            continue;
        mob.shootingAI->update(mob);
        if (!mob.shootingAI->isFiring())
            continue;
        auto aim = mob.shootingAI->getAim();
        mob.shootingAI->isFiring();
        mob.turret->position = mob.position;
        auto delta = aim - mob.position;
        float angle = atan2f(delta.x, delta.y);
        mob.turret->angle = angle;
        mob.turret->shoot(*teams.getTeamByID(mob.teamID));
    }
    //
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

void mobs::drawMobs(std::list<Mob>& mobs, const Camera& camera, const uint64_t deltaT) {
    if (Settings::gameplay.showHitboxes)
        drawHitboxes(mobs, camera);
    for (auto& mob : mobs) {
        if (!camera.contains(t1::tile(mob.position)))
            continue;
        if (mob.colided)
            mob.sprite.setPosition(mob.position);
        else
            mob.sprite.setPosition(mob.position + mob.velocity * (deltaT / 48.0f));
        mob.sprite.setRotationRad(mob.angle);
        mob.sprite.draw();
    }
}
