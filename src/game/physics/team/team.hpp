#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include "game/physics/mob.hpp"
#include "game/physics/shell.hpp"
#include "game/physics/physics_base.hpp"

class Camera;
class World;

class Team {
    std::u32string name;
    TeamID ID;
    std::list<Mob> mobs;
    std::list<Shell> shells;
public:
    Team(const std::u32string& name, const TeamID ID) : name(name), ID(ID) { }

    void interact(const World& world);
    void draw(const Camera& camera) const;

    void spawnShell(const ShellPreset& preset, const PixelCoord position, const float angle);

    std::list<Mob>& getMobs() { return mobs; }
    std::list<Shell>& getShells() { return shells; }

    const int getID() const { return ID; }
    const std::u32string& getName() const { return name; }
};
