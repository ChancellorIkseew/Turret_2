#include "block_map.hpp"
//
#include "game/common/teams_pool.hpp"

void BlockMap::updateBlocks(const WorldMap& terrain, const Presets& presets, TeamsPool& teams) {
    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            BlockTile& blockTile = at(x, y);
            const TileCoord tile = { x, y };
            switch (blockTile.type) {
            case BlockType::belt:
                static_cast<BeltBlock*>(blockTile.block.get())->update(tile, *this);
                break;
            case BlockType::drill:
                static_cast<DrillBlock*>(blockTile.block.get())->throwItem(tile, *this, terrain, presets);
                break;
            case BlockType::factory:
                static_cast<FactoryBlock*>(blockTile.block.get());
                break;
            case BlockType::intersection:
                static_cast<IntersectionBlock*>(blockTile.block.get())->provide(tile, *this);
                break;
            case BlockType::router:
                static_cast<RouterBlock*>(blockTile.block.get())->provide(tile, *this);
                break;
            case BlockType::turret:
                static_cast<TurretBlock*>(blockTile.block.get());
                break;
            case BlockType::core:
                static_cast<CoreBlock*>(blockTile.block.get())->syncTeam(teams.getTeamByID(blockTile.teamID));
                break;
            }
        }
    }
}
