#pragma once
#include "game/common/physics_base.hpp"
#include "inventory.hpp"

class Camera;
class World;

class Team {
    TeamID ID;
    Inventory inventory;
public:
    Team(const TeamID ID) : ID(ID) { }

    const int getID() const { return ID; }
    Inventory& getInventory() { return inventory; }
};
