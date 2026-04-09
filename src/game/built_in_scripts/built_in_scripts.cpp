#include "built_in_scripts.hpp"
//
#include "engine/assets/assets.hpp"
#include "engine/game_session.hpp"

void BuiltInScripts::execute(const TimeCount& timeCount) {
    if (timeCount.isWaveJustChanged())
        spawnWave(timeCount.getWaveCount());
}

void BuiltInScripts::spawnWave(const uint32_t wavenumber) {;
    const auto& presets = assets.getPresets();
    const Wave wave = assets.getWaves().getWave(wavenumber);
    const TeamID enemyTeam = 1; // refactor get wave

    for (size_t i = 0; i < wave.mob.size(); ++i) {
        const MobPresetID presetID = presets.getMobID(wave.mob[i]);
        const PixelCoord position(500, 500); // refactor get from ? 

        for (uint32_t j = 0; j < wave.amount[i]; ++j) {
            spawnMob(presetID, position, enemyTeam);
        }
    }
}

void BuiltInScripts::spawnMob(const MobPresetID presetID, const PixelCoord position, const TeamID teamID) {
    const auto& preset = assets.getPresets().getMob(presetID);
    auto& mobs = world.getMobs();
    MotionData mData(preset.defaultMovingAI, 0, PixelCoord(400, 1000));
    ShootingData sData(preset.defaultShootingAI, false, PixelCoord(0, 0));
    mobs.addMob(presetID, preset.turret, position, 0.0f, preset.maxHealth, teamID,
        preset.hitboxRadius, mData, sData, 0, 0.0f);
}

void BuiltInScripts::placeBlock(const BlockPresetID presetID, const TileCoord tile, const TeamID teamID) {
    auto& blocks = world.getBlocks();
    if (!blocks.isAir(tile))
        return;
    
    const auto& presets = assets.getPresets();
    const auto& preset = presets.getBlock(presetID);

    std::unique_ptr<Block> block = std::make_unique<CoreBlock>(); //temporary
    block->texture = preset.visual.texture;
    block->health = preset.maxHealth;
    blocks.at(tile).place(teamID, block);
}
