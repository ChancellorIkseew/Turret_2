#include "shells_system.hpp"
//
#include "game/player/camera.hpp"
#include"game/physics/mob_manager.hpp"
#include"game/physics/shell_manager.hpp"

static inline void countShellsLife(ShellSoA& soa) {
    for (size_t i = 0; i < soa.restLifeTime.size(); ++i) {
        --soa.restLifeTime[i];
    }
}

static inline void moveShells(ShellSoA& soa) {
    for (size_t i = 0; i < soa.position.size(); ++i) {
        soa.position[i] = soa.position[i] + soa.velocity[i];
    }
}

static inline void hitMobs(ShellSoA& shells, MobSoA& mobs) {
    for (size_t mob = 0; mob < mobs.id.size(); ++mob) {
        for (size_t shell = 0; shell < shells.position.size(); ++shell) {
            if (mobs.teamID[mob] == shells.teamID[shell])
                continue;
            if (!mobs.hitbox[mob].contains(shells.position[shell]))
                continue;
            if (mobs.health[mob] >= shells.restDamage[shell]) {
                mobs.health[mob] -= shells.restDamage[shell];
                shells.restDamage[shell] = 0;
            }
            else /*(health < restDamade)*/ {
                shells.restDamage[shell] -= mobs.health[mob];
                mobs.health[mob] = 0;
            }
        }
    }
}

void shells::processShells(ShellSoA& shells, MobSoA& mobs) {
    countShellsLife(shells);
    moveShells(shells);
    hitMobs(shells, mobs);
}

void shells::cleanupShells(ShellManager& manager/*, Explosions& explosions*/) {
    const auto& soa = manager.getSoa();
    // Reverse itaretion to avoid bugs with "swap and pop".
    for (size_t i = soa.position.size(); i > 0; --i) {
        size_t index = i - 1;
        if (soa.restLifeTime[index] > 0 && soa.restDamage[index] > 0)
            continue;
        // if (soa.presets->explosion.damage != 0)
        //     explosions.push(soa.presets->explosion);
        manager.removeShell(index);
    }
}

void shells::drawShells(const ShellSoA& soa, const Camera& camera, const float tickOfset) {
    Sprite sprite;
    for (size_t i = 0; i < soa.position.size(); ++i) {
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
