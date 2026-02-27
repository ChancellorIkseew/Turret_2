#include "block_archetype.hpp"

BlockArchetype block_archetype::getBlockArchetype(const std::string& name) {
    if (name == "wall") return BlockArchetype::wall;
    if (name == "drill") return BlockArchetype::drill;
    if (name == "factory") return BlockArchetype::factory;
    if (name == "turret") return BlockArchetype::turret;
    return BlockArchetype::air;
}
