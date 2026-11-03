#pragma once
#include "inventory.hpp"

class Team {
    Inventory inventory;
public:
    Team() = default;
    const Inventory& getInventory() const { return inventory; }
    Inventory& getInventory() { return inventory; }
};
