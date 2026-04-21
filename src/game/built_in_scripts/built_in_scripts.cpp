#include "built_in_scripts.hpp"
//
#include <random>
#include "engine/assets/assets.hpp"
#include "engine/game_session.hpp"
#include "game/blocks/make_block.hpp"

static PixelCoord randomMapBorderCoord(std::mt19937& gen, const TileCoord mapSize) {
    using IntRand = std::uniform_int_distribution<int>;
    const int side = IntRand(0, 3)(gen);
    const int x = IntRand(0, mapSize.x - 1)(gen);
    const int y = IntRand(0, mapSize.y - 1)(gen);
    //
    switch (side) {
    case  0: return t1::tileCenter(x, 0);
    case  1: return t1::tileCenter(0, y);
    case  2: return t1::tileCenter(x, mapSize.y - 1);
    default: return t1::tileCenter(mapSize.x - 1, y);
    }
}

void BuiltInScripts::execute(const TimeCount& timeCount) {
    if (timeCount.isWaveJustChanged())
        spawnWave(timeCount.getWaveCount());
    // target mobs motion to core
    const PixelCoord core = t1::tileCenter(world.getBlocks().getMeta().getCore());
    for (auto& motionData : world.getMobs().getSoa().motionData) {
        motionData.target = core;
    }
}

void BuiltInScripts::spawnWave(const uint32_t waveNumber) {
    const int seed = 0; // TODO: get seed from somewhere
    const TileCoord mapSize = world.getMap().getSize();
    std::mt19937 randomizer(waveNumber + seed);
    const PixelCoord position = randomMapBorderCoord(randomizer, mapSize);
    //
    const auto& presets = assets.getPresets();
    const Wave wave = assets.getWaves().getWave(waveNumber);
    const TeamID enemyTeam = 1; // refactor get team

    for (size_t i = 0; i < wave.mob.size(); ++i) {
        const MobPresetID presetID = presets.getMobID(wave.mob[i]);
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

void BuiltInScripts::placeBlock(const BlockPresetID presetID, const TileCoord tile, const TeamID teamID, BlockRot rotation) {
    auto& blocks = world.getBlocks();
    if (!blocks.isAir(tile))
        return;
    const auto& preset = assets.getPresets().getBlock(presetID);
    std::unique_ptr<Block> block = makeBlock(preset, rotation);
    blocks.place(tile, teamID, block);
}
