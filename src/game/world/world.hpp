#pragma once
#include "game/physics/mob_manager.hpp"
#include "game/physics/team/teams_pool.hpp"
#include "world_map.hpp"

class World {
    WorldMap map;
    TeamsPool teams;
    MobManager mobs;
public:
    World() : map(TileCoord(200, 200)) { }
    World(WorldMap& map) : map(std::move(map)), mobs(128) { }
    //
    void print();
    const WorldMap& getMap() const noexcept { return map; }
    WorldMap& getMap() noexcept { return map; }
    const TeamsPool& getTeams() const noexcept { return teams; }
    TeamsPool& getTeams() noexcept { return teams; }
    const MobManager& getMobs() const noexcept { return mobs; }
    MobManager& getMobs() noexcept { return mobs; }
};
