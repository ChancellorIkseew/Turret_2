#include "built_in_scripts.hpp"
//
#include <random>
#include "app.hpp"
#include "engine/gui/gui.hpp"
#include "game/blocks/make_block.hpp"
#include "game/frontend/frontend.hpp"
#include "game/game_session.hpp"
#include "game/world/world.hpp"

static PixelCoord randomMapBorderCoord(std::mt19937_64& gen, const TileCoord mapSize) {
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

void BuiltInScripts::execute(App& app, const TimeCount& timeCount) {
    if (app.getSession().getGameMode() == GameMode::survival) {
        if (timeCount.isWaveJustChanged()) {
            const WorldConfig& worldConfig = world.getConfig();
            app.getAssets().getAudio().playUI("wave_start");
            spawnWave(timeCount.getWaveCount(), worldConfig.enemyCountMul);
        }
        targetEnemies();
        respawnShuttle();
        const auto& cores = world.getBlocks().getMeta().getCores();
        if (cores.empty())
            app.getGUI().addToOverlay(frontend::initGameOver(app));
    }
}

void BuiltInScripts::targetEnemies() {
    const auto& cores = world.getBlocks().getMeta().getCores();
    auto& soa = world.getMobs().getSoa();
    const TeamID enemyTeam = 1; // refactor get team
    if (cores.empty()) {
        for (size_t i = 0; i < soa.mobCount; ++i) {
            if (soa.teamID[i] == enemyTeam)
                soa.motionData[i].target = soa.position[i];
        }
    }
    else {
        const TileCoord coreTile = cores[0];
        const PixelCoord halfCoreSize = t1::HALF_TILE_PC * static_cast<float>(world.getBlocks().at(coreTile).block->size);
        const PixelCoord coreCenter = t1::pixel(coreTile) + halfCoreSize;
        for (size_t i = 0; i < soa.mobCount; ++i) {
            if (soa.teamID[i] == enemyTeam)
                soa.motionData[i].target = coreCenter;
        }
    }
}

void BuiltInScripts::respawnShuttle() {
    const auto& cores = world.getBlocks().getMeta().getCores();
    if (cores.empty())
        return; // players core does not exist
    auto& soa = world.getMobs().getSoa();
    MobPresetID shuttle = assets.getPresets().getMobID("shuttle");
    const TeamID playerTeam = 0; // refactor get team
    for (size_t i = 0; i < soa.mobCount; ++i) {
        if (soa.teamID[i] == playerTeam && soa.preset[i] == shuttle)
            return; // shuttle is allive
    }
    spawnMob(shuttle, t1::tileCenter(cores[0]), playerTeam);
}

void BuiltInScripts::spawnWave(const uint32_t waveNumber, const uint32_t mobCountMul) {
    std::mt19937_64 randomizer(world.getConfig().seed + waveNumber);
    const PixelCoord position = randomMapBorderCoord(randomizer, world.getMap().getSize());
    //
    const auto& presets = assets.getPresets();
    const Wave wave = assets.getWaves().getWave(waveNumber);
    const TeamID enemyTeam = 1; // refactor get team

    for (size_t i = 0; i < wave.mob.size(); ++i) {
        const MobPresetID presetID = presets.getMobID(wave.mob[i]);
        const uint32_t amount = wave.amount[i] * mobCountMul;
        for (uint32_t j = 0; j < amount; ++j) {
            spawnMob(presetID, position, enemyTeam);
        }
    }
}

void BuiltInScripts::spawnMob(const MobPresetID presetID, const PixelCoord position, const TeamID teamID) {
    const auto& preset = assets.getPresets().getMob(presetID);
    auto& mobs = world.getMobs();
    MotionData mData(preset.defaultMovingAI, 0, PixelCoord(400, 1000));
    ShootingData sData(preset.defaultShootingAI, false, PixelCoord(0, 0));
    mobs.addMob(presetID, preset.turret, position, 0.0f, preset.maxHealth, preset.maxShieldHealth,
        teamID, preset.hitboxRadius, mData, sData, 0, 0.0f);
}

void BuiltInScripts::placeBlock(const BlockPresetID presetID, const TileCoord tile, const TeamID teamID, const BlockRot rotation) {
    auto& blocks = world.getBlocks();
    const auto& preset = assets.getPresets().getBlock(presetID);
    if(!blocks.canPlace(tile, preset.size))
        return;
    std::unique_ptr<Block> block = makeBlock(assets.getPresets(), presetID, preset, rotation);
    blocks.place(tile, teamID, block);
}
