#pragma once
#include "engine/assets/preset_defs.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "game/blocks/block.hpp"
#include "game/common/physics_base.hpp"

class App;
class Assets;
class TimeCount;
class World;

class BuiltInScripts {
    const Assets& assets;
    World& world;
public:
    BuiltInScripts(const Assets& assets, World& world) : assets(assets), world(world) { }

    void execute(App& app, const TimeCount& timeCount);
    void spawnWave(const uint32_t waveNumber, const uint32_t mobCountMul);
    void targetEnemies();
    void respawnShuttle();

    void spawnMob(const MobPresetID presetID, const PixelCoord position, const TeamID teamID);
    void placeBlock(const BlockPresetID presetID, const TileCoord tile, const TeamID teamID, const BlockRot rotation);
private:
    t1_disable_copy_and_move(BuiltInScripts)
};
