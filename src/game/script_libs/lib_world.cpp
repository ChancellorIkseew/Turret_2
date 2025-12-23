#include "script_libs.hpp"
//
#include "engine/debug/logger.hpp"
#include "game/content/presets.hpp"
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
    if (!content::Presets::getMobs().contains(presetName)) {
        logger.warning() << "Preset does not exist. Preset: " << presetName;
        return;
    }
    if (!script_libs::world->getTeams().containsID(teamID)) {
        logger.warning() << "Team with ID does not exist. TeamID: " << static_cast<int>(teamID);
        return;
    }
    auto& preset = content::Presets::getMobs().at(presetName);
    script_libs::world->getMobs().addMob(preset, PixelCoord(x, y), 0.f, preset->maxHealth, teamID);
}

static void spawnShell(const std::string& presetName, int x, int y, AngleRad angle, TeamID teamID) {
    if (!content::Presets::getShells().contains(presetName)) {
        logger.warning() << "Preset does not exist. Preset: " << presetName;
        return;
    }
    if (!script_libs::world->getTeams().containsID(teamID)) {
        logger.warning() << "Team with ID does not exist. TeamID: " << static_cast<int>(teamID);
        return;
    }
    auto& preset = content::Presets::getShells().at(presetName);
    script_libs::world->getShells().addShell(preset, PixelCoord(x, y), angle, preset->damage, preset->maxLifeTime, teamID);
}

void script_libs::registerWorld(const ScriptsHandler& scriptsHandler) {
    scriptsHandler.registerFunction("int world_getWidth()", asFunctionPtr(getWidth));
    scriptsHandler.registerFunction("int world_getHeight()", asFunctionPtr(getHeight));
    scriptsHandler.registerFunction("void world_placeFloor(int x, int y, uint8 floorID)", asFunctionPtr(placeFloor));
    scriptsHandler.registerFunction("void world_placeOverlay(int x, int y, uint8 overlayID)", asFunctionPtr(placeFloor));
    scriptsHandler.registerFunction("void world_spawnMob(string preset, int x, int y, uint8 teamID)", asFunctionPtr(spawnMob));
    scriptsHandler.registerFunction("void world_spawnShell(string preset, int x, int y, float angle, uint8 teamID)", asFunctionPtr(spawnShell));
}
