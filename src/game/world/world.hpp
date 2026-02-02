#pragma once
#include "game/blocks/blocks.hpp"
#include "game/physics/chunk_grid.hpp"
#include "game/physics/mob_manager.hpp"
#include "game/physics/shell_manager.hpp"
#include "game/physics/team/teams_pool.hpp"
#include "world_map.hpp"

class World {
    Blocks blocks;
    ChunkGrid chunkGrid;
    //
    WorldMap map;
    TeamsPool teams;
    MobManager mobs;
    ShellManager shells;
public:
    World(WorldMap& map) :
        chunkGrid(map.getSize()),
        blocks(map.getSize()),
        map(std::move(map)),
        mobs(128),
        shells(256) { }
    //
    const WorldMap& getMap() const noexcept { return map; }
    const Blocks& getBlocks() const noexcept { return blocks; }
    const TeamsPool& getTeams() const noexcept { return teams; }
    const ChunkGrid& getChunks() const noexcept { return chunkGrid; }
    const MobManager& getMobs() const noexcept { return mobs; }
    const ShellManager& getShells() const noexcept { return shells; }
    WorldMap& getMap() noexcept { return map; }
    Blocks& getBlocks() noexcept { return blocks; }
    TeamsPool& getTeams() noexcept { return teams; }
    ChunkGrid& getChunks() noexcept { return chunkGrid; }
    MobManager& getMobs() noexcept { return mobs; }
    ShellManager& getShells() noexcept { return shells; }
};
