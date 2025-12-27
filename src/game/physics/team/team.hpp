#pragma once
#include <list>
#include <string>
#include "game/physics/physics_base.hpp"

class Camera;
class World;

class Team {
    std::u32string name;
    TeamID ID;
public:
    Team(const std::u32string& name, const TeamID ID) : name(name), ID(ID) { }

    const int getID() const { return ID; }
    const std::u32string& getName() const { return name; }
};
