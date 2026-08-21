#include "block_map.hpp"
//
#include "engine/assets/presets.hpp"
#include "game/common/teams_pool.hpp"

void BlockMap::updateBlocks(const WorldMap& terrain, const Presets& presets, TeamsPool& teams, const uint64_t tickCount) {
    const bool tickForUpdate = (tickCount % 60) == 0;

    for (int x = 0; x < mapSize.x; ++x) {
        for (int y = 0; y < mapSize.y; ++y) {
            BlockTile& blockTile = at(x, y);
            const TileCoord tile = { x, y };
            switch (blockTile.type) {
            case BlockType::belt:
                static_cast<BeltBlock*>(blockTile.block.get())->update(tile, *this);
                break;
            case BlockType::drill:
                if (tickForUpdate)
                    static_cast<DrillBlock*>(blockTile.block.get())->mine(tile, terrain, presets);
                static_cast<DrillBlock*>(blockTile.block.get())->provide(tile, *this);
                break;
            case BlockType::factory:
                static_cast<FactoryBlock*>(blockTile.block.get());
                break;
            case BlockType::junction:
                static_cast<JunctionBlock*>(blockTile.block.get())->provide(tile, *this);
                break;
            case BlockType::router:
                static_cast<RouterBlock*>(blockTile.block.get())->provide(tile, *this);
                break;
            case BlockType::core:
                static_cast<CoreBlock*>(blockTile.block.get())->syncTeam(teams.getTeamByID(blockTile.teamID));
                break;
            }
        }
    }

    auto& turrets = meta.getTurrets().getSoa();
    for (size_t i = 0; i < turrets.turretCount; ++i) {
        if (turrets.ammo[i] > 0 || at(turrets.masterTile[i]).type != BlockType::turret) // mb temporary 
            continue;
        TurretBlock* block = static_cast<TurretBlock*>(at(turrets.masterTile[i]).block.get());
        if (block->ammo.count > 0) {
            block->useAmmo();
            turrets.ammo[i] += presets.getTurret(turrets.preset[i]).ammoByItem;
        }
    }

    meta.setCoreAttacked(false);
    const auto& cores = meta.getCores();
    for (TileCoord tile : cores) {
        CoreBlock* coreBlock = static_cast<CoreBlock*>(at(tile).block.get());
        if (coreBlock->health < coreBlock->preveouseTickHealth)
            meta.setCoreAttacked(true);
        coreBlock->preveouseTickHealth = coreBlock->health;
    }
}
