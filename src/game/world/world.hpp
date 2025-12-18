#pragma once
#include "game/physics/mob_soa.hpp"
#include "game/physics/team/teams_pool.hpp"
#include "world_map.hpp"

class World {
    WorldMap map;
    TeamsPool teams;
    MobSoA mobs;
public:
    World() : map(TileCoord(200, 200)) { }
    World(WorldMap& map) : map(std::move(map)) {
        reserve(mobs, 128);
    }
    void print();
    const WorldMap& getMap() const { return map; }
    WorldMap& getMap() { return map; }
    const TeamsPool& getTeams() const { return teams; }
    TeamsPool& getTeams() { return teams; }
    const MobSoA& getMobs() const { return mobs; }
    MobSoA& getMobs() { return mobs; }
};
