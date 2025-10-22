#pragma once
#include <list>
#include <string>
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

    void interact(World& world);
    void drawGroundUnits(const Camera& camera, const uint64_t deltaT);
    void drawAirUnits(const Camera& camera, const uint64_t deltaT);
    void drawShells(const Camera& camera, const uint64_t deltaT);

    void spawnMob(const MobPreset& preset, const PixelCoord position, const float angle);
    void spawnShell(const ShellPreset& preset, const PixelCoord position, const float angle);


    const std::list<Mob>& getMobs() const { return mobs; }
    const std::list<Shell>& getShells() const { return shells; }
    std::list<Mob>& getMobs() { return mobs; }
    std::list<Shell>& getShells() { return shells; }

    const int getID() const { return ID; }
    const std::u32string& getName() const { return name; }
};
