#pragma once
#include <list>
#include <string>
#include "game/common/physics_base.hpp"
#include "inventory.hpp"

class Camera;
class World;

class Team {
    std::u32string name;
    TeamID ID;
    Inventory inventory;
public:
    Team(const std::u32string& name, const TeamID ID) : name(name), ID(ID) { }

    const int getID() const { return ID; }
    const std::u32string& getName() const { return name; }
    Inventory& getInventory() { return inventory; }
};
