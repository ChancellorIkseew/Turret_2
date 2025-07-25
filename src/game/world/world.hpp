#pragma once
#include "game/physics/team/teams_pool.hpp"
#include "world_map.hpp"

class World {
    WorldMap map;
    TeamsPool teams;
public:
    World(const TileCoord mapSize) : map(mapSize) { }
    void print();
    const WorldMap& getMap() const { return map; }
    WorldMap& getMap() { return map; }
    const TeamsPool& getTeams() const { return teams; }
    TeamsPool& getTeams() { return teams; }
};
