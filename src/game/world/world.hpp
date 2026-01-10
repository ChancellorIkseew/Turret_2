#pragma once
#include "game/physics/chunk_grid.hpp"
#include "game/physics/mob_manager.hpp"
#include "game/physics/shell_manager.hpp"
#include "game/physics/team/teams_pool.hpp"
#include "world_map.hpp"

class World {
    ChunkGrid chunkGrid;
    //
    WorldMap map;
    TeamsPool teams;
    MobManager mobs;
    ShellManager shells;
public:
    World(WorldMap& map) :
        chunkGrid(map.getSize()),
        map(std::move(map)),
        mobs(128),
        shells(256) { }
    //
    void print();
    const WorldMap& getMap() const noexcept { return map; }
    const TeamsPool& getTeams() const noexcept { return teams; }
    const ChunkGrid& getChunks() const noexcept { return chunkGrid; }
    const MobManager& getMobs() const noexcept { return mobs; }
    const ShellManager& getShells() const noexcept { return shells; }
    WorldMap& getMap() noexcept { return map; }
    TeamsPool& getTeams() noexcept { return teams; }
    ChunkGrid& getChunks() noexcept { return chunkGrid; }
    MobManager& getMobs() noexcept { return mobs; }
    ShellManager& getShells() noexcept { return shells; }
};
