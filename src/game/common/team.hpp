#pragma once
#include "inventory.hpp"

class Team {
    Inventory inventory;
public:
    Team() = default;
    Inventory& getInventory() { return inventory; }
};
