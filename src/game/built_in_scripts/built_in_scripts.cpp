#include "built_in_scripts.hpp"
//
#include "engine/engine.hpp"
#include "engine/game_session.hpp"

static void spawnMobs(const uint32_t waveCount, MobManager& mobs, const Assets& assets) {
    const auto& presets = assets.getPresets();
    const auto& waves = assets.getWaves();
    const Wave wave = waves.getWave(waveCount);
    for (size_t i = 0; i < wave.mob.size(); ++i) {
        const MobPresetID presetID = presets.getMobID(wave.mob[i]);
        const MobPreset& preset = presets.getMob(presetID);
        const TeamID enemyTeam = 1;
        MotionData mData(preset.defaultMovingAI, 0, PixelCoord(400, 1000));
        ShootingData sData(preset.defaultShootingAI, false, PixelCoord(0, 0));
        const PixelCoord position(500, 500);

        for (uint32_t j = 0; j < wave.amount[i]; ++j) {
            mobs.addMob(presetID, preset.turret, position, 0.0f, preset.maxHealth, enemyTeam, preset.hitboxRadius,
                mData, sData, 0, 0.0f);
        }
    }
}

void built_in_scripts::execute(Engine& engine) {
    const auto& tickCount = engine.getSession().getTimeCount();
    auto& mobs = engine.getSession().getWorld().getMobs();
    if (tickCount.isWaveJustChanged())
        spawnMobs(tickCount.getWaveCount(), mobs, engine.getAssets());
}
