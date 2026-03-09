#include "script_libs.hpp"
//
#include "engine/assets/assets.hpp"
#include "engine/debug/logger.hpp"
#include "game/world/world.hpp"

static debug::Logger logger("scripts_lib_world");

static void placeFloor(int x, int y, uint8_t floorID) {
    script_libs::world->getMap().placeFloor(TileCoord(x, y), floorID);
}
static void placeOverlay(int x, int y, uint8_t overlayID) {
    script_libs::world->getMap().placeOverlay(TileCoord(x, y), overlayID);
}

static int getWidth() {
    return script_libs::world->getMap().getSize().x;
}
static int getHeight() {
    return script_libs::world->getMap().getSize().y;
}

static void spawnMob(const std::string& presetName, int x, int y, TeamID teamID) {
    if (!script_libs::assets->getPresets().hasMobID(presetName)) {
        logger.warning() << "Preset does not exist. Preset: " << presetName;
        return;
    }
    if (!script_libs::world->getTeams().containsID(teamID)) {
        logger.warning() << "Team with ID does not exist. TeamID: " << static_cast<int>(teamID);
        return;
    }

    const MobPresetID presetID = script_libs::assets->getPresets().getMobID(presetName);
    script_libs::builtInScripts->spawnMob(presetID, { x, y }, teamID);
}

static void spawnShell(const std::string& presetName, int x, int y, AngleRad angle, TeamID teamID) {
    if (!script_libs::assets->getPresets().hasShellID(presetName)) {
        logger.warning() << "Preset does not exist. Preset: " << presetName;
        return;
    }
    if (!script_libs::world->getTeams().containsID(teamID)) {
        logger.warning() << "Team with ID does not exist. TeamID: " << static_cast<int>(teamID);
        return;
    }
    const Presets& presets = script_libs::assets->getPresets();
    ShellPresetID presetID = presets.getShellID(presetName);
    const ShellPreset& preset = presets.getShell(presetID);
    script_libs::world->getShells().addShell(presets, presetID, PixelCoord(x, y), angle, preset.damage,
        preset.maxLifeTime, teamID);
}

void script_libs::registerWorld(const ScriptsHandler& scriptsHandler) {
    scriptsHandler.registerFunction("int world_getWidth()", asFunctionPtr(getWidth));
    scriptsHandler.registerFunction("int world_getHeight()", asFunctionPtr(getHeight));
    scriptsHandler.registerFunction("void world_placeFloor(int x, int y, uint8 floorID)", asFunctionPtr(placeFloor));
    scriptsHandler.registerFunction("void world_placeOverlay(int x, int y, uint8 overlayID)", asFunctionPtr(placeFloor));
    scriptsHandler.registerFunction("void world_spawnMob(string preset, int x, int y, uint8 teamID)", asFunctionPtr(spawnMob));
    scriptsHandler.registerFunction("void world_spawnShell(string preset, int x, int y, float angle, uint8 teamID)", asFunctionPtr(spawnShell));
}
