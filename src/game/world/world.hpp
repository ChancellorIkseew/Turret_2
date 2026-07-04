#pragma once
#include "game/blocks/block_map.hpp"
#include "game/common/teams_pool.hpp"
#include "game/entities/chunk_grid.hpp"
#include "game/entities/mobs_pool.hpp"
#include "game/entities/particles_pool.hpp"
#include "game/entities/shells_pool.hpp"
#include "game/frontend/build_tools/blueprint.hpp"
#include "world_map.hpp"

class World {
    BlockMap blocks;
    ChunkGrid chunkGrid;
    //
    WorldMap map;
    TeamsPool teams;
    MobsPool mobs;
    ShellsPool shells;
    ParticlesPool particles;
    Blueprints blueprints;
public:
    World(WorldMap& map) :
        blocks(map.getSize()),
        map(std::move(map)),
        mobs(128),
        shells(256) { }
    //
    const WorldMap& getMap() const noexcept { return map; }
    const BlockMap& getBlocks() const noexcept { return blocks; }
    const TeamsPool& getTeams() const noexcept { return teams; }
    const ChunkGrid& getChunks() const noexcept { return chunkGrid; }
    const MobsPool& getMobs() const noexcept { return mobs; }
    const ShellsPool& getShells() const noexcept { return shells; }
    const ParticlesPool& getParticles() const noexcept { return particles; }
    WorldMap& getMap() noexcept { return map; }
    BlockMap& getBlocks() noexcept { return blocks; }
    TeamsPool& getTeams() noexcept { return teams; }
    ChunkGrid& getChunks() noexcept { return chunkGrid; }
    MobsPool& getMobs() noexcept { return mobs; }
    ShellsPool& getShells() noexcept { return shells; }
    ParticlesPool& getParticles() noexcept { return particles; }
    Blueprints& getBlueprints() noexcept { return blueprints; }
};
