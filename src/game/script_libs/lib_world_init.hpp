#pragma once
#include "game/world/world.hpp"

namespace lib_world {
    inline World* world;
    inline void init(World* world) {
        lib_world::world = world;
    }
}
