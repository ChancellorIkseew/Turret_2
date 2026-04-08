#pragma once
#include <memory>
#include <vector>
#include "game/physics/id_manager.hpp"
#include "game/physics/physics_base.hpp"
#include "block.hpp"

constexpr TeamID INVALID_TEAM_ID = IDManager<TeamID>::INVALID_ID;

struct BlockTile {
    TeamID teamID = INVALID_TEAM_ID;
    BlockType type = BlockType::air;
    std::unique_ptr<Block> block;
    //
    BlockTile() = default;
    BlockTile(TeamID teamID, std::unique_ptr<Block>& block) :
        teamID(teamID), type(block->getType()), block(std::move(block)) {}

    void place(TeamID teamID, std::unique_ptr<Block>& block) {
        this->teamID = teamID;
        this->type = block->getType();
        this->block = std::move(block);
    }

    void demolish() {
        teamID = INVALID_TEAM_ID;
        type = BlockType::air;
        block.reset();
    }
};

class BlockMap {
    std::vector<BlockTile> blocks;
    TileCoord mapSize;
public:
    BlockMap(TileCoord mapSize) : mapSize(mapSize) {
        blocks.assign(mapSize.x * mapSize.y, BlockTile());
    }
    //
    BlockTile& at(const int x, const int y) noexcept { return blocks[x + y * mapSize.x]; }
    BlockTile& at(const TileCoord tile)     noexcept { return at(tile.x, tile.y); }
    TileCoord getSize() const noexcept { return mapSize; }
    auto& getRaw() noexcept { return blocks; }
};

void update(BlockMap& map) {
    const TileCoord size = map.getSize();
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            BlockTile& blockTile = map.at(x, y);
            switch (blockTile.type) {
            case BlockType::drill:
                static_cast<DrillBlock*>(blockTile.block.get());
                // do sth
                break;
            case BlockType::factory:
                static_cast<FactoryBlock*>(blockTile.block.get());
                // do sth
                break;
            case BlockType::turret:
                static_cast<TurretBlock*>(blockTile.block.get());
                // do sth
                break;
            }
        }
    }
}
