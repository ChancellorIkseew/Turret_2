#include "game/blocks/block_types.hpp"
//
#include "game/blocks/block_map.hpp"

void TurretBlock::onPlace(BlocksMeta& meta, TileCoord tile, TeamID teamID, BlockRot rotation) {
    const AngleRad angleRad = static_cast<float>(rotation) * t1::TAU;
    const PixelCoord origin = t1::HALF_TILE_PC * static_cast<float>(size);
    meta.getTurrets().addTurret(turretPreset, tile, t1::pixel(tile) + origin, t1::PI - angleRad, teamID, ShootingData(), 0);
}

void TurretBlock::onDemolish(BlocksMeta& meta, TileCoord tile) {
	meta.getTurrets().removeByMasterTile(tile);
}
