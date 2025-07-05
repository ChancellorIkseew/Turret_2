#include "physics.hpp"
//
#include <vector>
#include "config.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "engine/coords/transforms.hpp"


struct Hitbox {
    PixelCoord start;
    PixelCoord end;

    t1_finline bool contains(const PixelCoord pixel) const noexcept {
        return pixel.x >= start.x && pixel.x <= end.x &&
               pixel.y >= start.y && pixel.y <= end.y;
    }

    t1_finline void move(const PixelCoord vector) const noexcept {
        start = start + vector;
        end = end + vector;
    }
};


struct Shell {
    PixelCoord pixelCoord;
    Hitbox hitbox;
    bool wasted = false;
    int32_t damage;
};

struct Mob {
    Hitbox hitbox;
    bool wasted = false;
    int32_t health = 100;

    void setDamage(const int32_t damage) {
        health -= damage;
        if (health < 1)
            wasted = true;
    }
};

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
    for (Shell* shell : chunk.shells) {
        if (!shell)
            continue;
        if (map.hasBuilding(t1::tile(shell->pixelCoord))) {
            shell->wasted = true;
            //map->building.setDamage();
            break;
        }
        for (Mob* mob : chunk.mobs) {
            if (mob && mob->hitbox.contains(shell->pixelCoord)) {
                shell->wasted = true;
                mob->setDamage(shell->damage);
                break;
            }
        }
    }
}



class Building;

struct MapTile {
    uint8_t tileType;
    uint8_t resType;
    Building* building;
};
