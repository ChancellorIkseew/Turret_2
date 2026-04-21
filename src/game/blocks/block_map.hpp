#pragma once
#include <memory>
#include <vector>
#include "game/common/id_manager.hpp"
#include "game/common/physics_base.hpp"
#include "block.hpp"

#include "engine/coords/transforms.hpp"
#include "game/entities/turret_manager.hpp"

class BlocksMeta {
    TurretManager turrets;
    std::vector<size_t> markedForRemove;
    TileCoord core = {50, 50}; // temp
public:
    BlocksMeta() : turrets(64) {}
    //
    auto& getTurrets() { return turrets; }
    //
    void markForRemove(TileCoord tile) {
        auto& soa = turrets.getSoa();
        for (size_t i = 0; i < soa.turretCount; ++i) {
            if (tile == t1::tile(soa.position[i]))
                return markedForRemove.push_back(i);
        }
    }
    void cleanUp() {
        if (markedForRemove.empty())
            return;
        std::ranges::sort(markedForRemove, std::greater<size_t>());
        auto [first, last] = std::ranges::unique(markedForRemove);
        markedForRemove.erase(first, last);
        for (size_t targetIndex : markedForRemove) {
            turrets.removeTurret(targetIndex);
        }
        markedForRemove.clear();
    }

    TileCoord getCore() const { return core; }
};

constexpr TeamID INVALID_TEAM_ID = IDManager<TeamID>::INVALID_ID;

struct BlockTile {
    TeamID teamID = INVALID_TEAM_ID;
    BlockType type = BlockType::air;
    std::unique_ptr<Block> block;
    //
    BlockTile() = default;
    BlockTile(TeamID teamID, std::unique_ptr<Block> block) :
        teamID(teamID), block(std::move(block)) {
        if (this->block)
            type = this->block->getType();
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
    BlocksMeta meta;
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
    auto& getMeta() noexcept { return meta; }
    //
    bool contains(TileCoord tile) const noexcept { return t1::contains(TileCoord(0, 0), mapSize - TileCoord(1, 1), tile); }
    bool isFilled(TileCoord tile) const noexcept { return contains(tile) && at(tile).type != BlockType::air; }
    bool isAir(TileCoord tile)    const noexcept { return contains(tile) && at(tile).type == BlockType::air; }
    //
    void place(TileCoord tile, TeamID teamID, std::unique_ptr<Block>& block) {
        if (block->getType() == BlockType::turret) {
            TurretPresetID preset = static_cast<TurretBlock*>(block.get())->turretPreset;
            meta.getTurrets().addTurret(preset, t1::tileCenter(tile), 0.0f, teamID, ShootingData(), 0);
        }
        at(tile).place(teamID, block);
    }
    void demolish(TileCoord tile) {
        if (at(tile).type == BlockType::turret)
            meta.markForRemove(tile);
        at(tile).demolish();
    }

    t1_disable_copy_and_move(BlockMap)
};

inline void updateBlocks(BlockMap& map, const WorldMap& terrain) { // replace later
    const TileCoord size = map.getSize();
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            BlockTile& blockTile = map.at(x, y);
            switch (blockTile.type) {
            case BlockType::belt:
                static_cast<BeltBlock*>(blockTile.block.get())->update({ x, y }, map);
                break;
            case BlockType::drill:
                static_cast<DrillBlock*>(blockTile.block.get())->throwItem({ x, y }, map, terrain);
                break;
            case BlockType::factory:
                static_cast<FactoryBlock*>(blockTile.block.get());
                break;
            case BlockType::router:
                static_cast<RouterBlock*>(blockTile.block.get())->provide({ x,y }, map);
                break;
            case BlockType::turret:
                static_cast<TurretBlock*>(blockTile.block.get());
                break;
            }
        }
    }
}
