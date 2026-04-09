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
    BlockTile(TeamID teamID, std::unique_ptr<Block> block) :
        teamID(teamID), block(std::move(block)) {
        if (block)
            type = block->getType();
    }

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

    BlockTile(BlockTile&&) noexcept = default;
    BlockTile& operator=(BlockTile&&) noexcept = default;
    t1_disable_copy(BlockTile)
};

class BlockMap {
    std::vector<BlockTile> blocks;
    TileCoord mapSize;
public:
    BlockMap(TileCoord mapSize) : mapSize(mapSize) {
        blocks.resize(mapSize.x * mapSize.y);
    }
    //
    const BlockTile& at(int x, int y)   const noexcept { return blocks[x + y * mapSize.x]; }
    const BlockTile& at(TileCoord tile) const noexcept { return at(tile.x, tile.y); }
    BlockTile& at(int x, int y)   noexcept { return blocks[x + y * mapSize.x]; }
    BlockTile& at(TileCoord tile) noexcept { return at(tile.x, tile.y); }
    TileCoord getSize() const noexcept { return mapSize; }
    auto& getRaw() noexcept { return blocks; }
    //
    bool contains(TileCoord tile) const noexcept { return t1::contains(TileCoord(0, 0), mapSize - TileCoord(1, 1), tile); }
    bool isFilled(TileCoord tile) const noexcept { return contains(tile) && at(tile).type == BlockType::air; }
    bool isAir(TileCoord tile)    const noexcept { return contains(tile) && at(tile).type != BlockType::air;
    }
    t1_disable_copy_and_move(BlockMap)
};

inline void update(BlockMap& map) { // replace later
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
