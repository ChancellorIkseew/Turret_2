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

static inline void resolveCollisions(Mob& mob, std::list<Mob>::iterator current, std::list<Mob>::iterator end) {
    for (auto it = std::next(current); it != end; ++it) {
        Mob& other = *it;

        if (!mob.hitbox.intersects(other.hitbox)) continue;

        mob.colided = true;
        other.colided = true;

        const PixelCoord overlap = mob.hitbox.overlap(other.hitbox);

        const bool pushX = (overlap.x < overlap.y);
        const float distance = pushX ? overlap.x : overlap.y;

        const float pushDirection = pushX ? (mob.position.x > other.position.x ? 1.0f : -1.0f)
            : (mob.position.y > other.position.y ? 1.0f : -1.0f);

        const float totalRadius = mob.preset.hitboxRadius + other.preset.hitboxRadius;
        const float invertedTotalRadius = 1.0f / totalRadius;
        const float mob1Weight = other.preset.hitboxRadius * invertedTotalRadius;
        const float mob2Weight = mob.preset.hitboxRadius * invertedTotalRadius;

        PixelCoord pushVec;
        if (pushX) pushVec.x = distance * pushDirection;
        else       pushVec.y = distance * pushDirection;

        move(mob, pushVec * mob1Weight);
        move(other, pushVec * -mob2Weight);
    }
}

void mobs::processMobs(std::list<Mob>& mobs, TeamsPool& teams) { 
    for (auto& mob : mobs) {
        mob.colided = false; //TODO: find better way to resolve collisions.
    }
    for (auto it = mobs.begin(); it != mobs.end(); ++it) {
        resolveCollisions(*it, it, mobs.end());
    }
    for (auto& mob : mobs) {
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

void mobs::drawMobs(std::list<Mob>& mobs, const Camera& camera, const float tickOfset) {
    if (Settings::gameplay.showHitboxes)
        drawHitboxes(mobs, camera);
    for (auto& mob : mobs) {
        if (!camera.contains(t1::tile(mob.position)))
            continue;
        if (mob.colided)
            mob.sprite.setPosition(mob.position);
        else
            mob.sprite.setPosition(mob.position + mob.velocity * tickOfset);
        mob.sprite.setRotationRad(mob.angle);
        mob.sprite.draw();
    }
}
