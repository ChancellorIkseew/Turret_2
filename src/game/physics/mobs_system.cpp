#include "mobs_system.hpp"
//
#include <mutex>
#include "engine/coords/transforms.hpp"
#include "game/world/camera.hpp"
#include "team/teams_pool.hpp"

static std::mutex mobsMutex;
static Sprite mobSprite;

void mobs::processMobs(std::list<Mob>& mobs, TeamsPool& teams) {
    for (auto& mob : mobs) {
        (*mob.AI)(mob);
        mob.position = mob.position + mob.velocity;
    }
    //
    std::lock_guard<std::mutex> guard(mobsMutex);
    mobs.remove_if([](const Mob& mob) { return mob.wasted; });
}

void mobs::drawMobs(const std::list<Mob>& mobs, const Camera& camera) {
    std::lock_guard<std::mutex> guard(mobsMutex);
    for (auto& mob : mobs) {
        if (!camera.contains(t1::tile(mob.position)))
            continue;
        mobSprite.setTexture(Texture(mob.preset.textureName));
        mobSprite.setSize(PixelCoord(45, 45));
        mobSprite.setPosition(mob.position);
        mobSprite.setRotation(mob.angle);
        mobSprite.draw();
    }
}
