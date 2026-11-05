#pragma once
#include "engine/assets/preset_defs.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "game/blocks/block.hpp"
#include "game/common/physics_base.hpp"

class App;
class Assets;
class World;

class BuiltInScripts {
    const Assets& assets;
    World& world;
    TeamID playerTeamID = 0, enemyTeamID = 0;
public:
    BuiltInScripts(const Assets& assets, World& world) : assets(assets), world(world) { }
    void initTeamIDs(const TeamID player, const TeamID enemy) { playerTeamID = player; enemyTeamID = enemy; }

    void execute(App& app);
    void spawnWave(const uint32_t waveNumber, const uint32_t mobCountMul);
    void targetEnemies();
    void respawnShuttle();

    void spawnMob(const MobPresetID presetID, const PixelCoord position, const TeamID teamID);
    void placeBlock(const BlockPresetID presetID, const TileCoord tile, const TeamID teamID, const BlockRot rotation);
private:
    t1_disable_copy_and_move(BuiltInScripts)
};
