#include "shells_system.hpp"
//
#include <mutex>
#include "engine/coords/transforms.hpp"
#include "game/world/camera.hpp"
#include "team/teams_pool.hpp"

static std::mutex shellsMutex;
static Sprite shellSprite;

static inline void tryHitBuilding(Shell& shell, TeamsPool& teams) {
}

static inline void tryHitMob(Shell& shell, TeamsPool& teams) {
    for (auto& [teamID, team] : teams) {
        if (shell.teamID == teamID)
            continue;
        for (auto& mob : team->getMobs()) {
            if (!mob.hitbox.contains(shell.position))
                continue;
            mob.health -= shell.preset.directDamage;
            shell.wasted = true;
            return;
        }
    }
}

void shells::processShells(std::list<Shell>& shells, TeamsPool& teams) {
    for (auto& shell : shells) {
        shell.position = shell.position + shell.velocity;
        if (--shell.restLifeTime <= 0)
            shell.wasted = true;
        tryHitMob(shell, teams);
    }
    //
    std::lock_guard<std::mutex> guard(shellsMutex);
    shells.remove_if([](const Shell& shell) { return shell.wasted; });
}

void shells::drawShells(const std::list<Shell>& shells, const Camera& camera) {
    std::lock_guard<std::mutex> guard(shellsMutex);
    for (auto& shell : shells) {
        if (!camera.contains(t1::tile(shell.position)))
            continue;
        shellSprite.setPosition(shell.position);
        shellSprite.setRotation(shell.angle);
        shellSprite.draw();
    }
}
