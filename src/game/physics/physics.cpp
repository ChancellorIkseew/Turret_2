#include "physics.hpp"
//
#include <vector>
#include "engine/coords/transforms.hpp"
#include "mob.hpp"
#include "shell.hpp"

struct Map {
    bool hasBuilding(const TileCoord tile) {
        return false;
    }
};

struct Chunk {
    std::vector<Mob*> mobs;
    std::vector<Shell*> shells;
};

void physics::procHitboxes() {
    Map map;
    Chunk chunk;
    for (auto& shell : chunk.shells) {
        if (!shell)
            continue;
        if (map.hasBuilding(t1::tile(shell->position))) {
            shell->wasted = true;
            //map->building.setDamage();
            break;
        }
        for (Mob* mob : chunk.mobs) {
            if (mob && mob->hitbox.contains(shell->position)) {
                shell->wasted = true;
                mob->health -= shell->preset.directDamage;
                break;
            }
        }
    }
}
