#include "script_libs.hpp"
//
#include "engine/debug/logger.hpp"
//#include "game/presets/mob_presets.hpp"
//#include "game/presets/shell_presets.hpp"
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

static void spawnMob(int x, int y, TeamID teamID) {
    if (!script_libs::world->getTeams().containsID(teamID)) {
        logger.warning() << "Team with ID does not exist. TeamID: " << static_cast<int>(teamID);
        return;
    }   
    auto team = script_libs::world->getTeams().getTeamByID(teamID);
    //team->spawnMob(cannonBoss, PixelCoord(x, y), 0.0f);
    //team->spawnMob(gatlingBot, PixelCoord(x + 50, y), 0.0f);
    //team->spawnMob(shuttle, PixelCoord(x + 100, y), 0.0f);
}

static void spawnShell(int x, int y, TeamID teamID) {
    if (!script_libs::world->getTeams().containsID(teamID)) {
        logger.warning() << "Team with ID does not exist. TeamID: " << static_cast<int>(teamID);
        return;
    }
    auto team = script_libs::world->getTeams().getTeamByID(teamID);
    //team->spawnShell(cannonShell, PixelCoord(x, y), 0.5f);
}

void script_libs::registerWorld(const ScriptsHandler& scriptsHandler) {
    scriptsHandler.registerFunction("int world_getWidth()", asFunctionPtr(getWidth));
    scriptsHandler.registerFunction("int world_getHeight()", asFunctionPtr(getHeight));
    scriptsHandler.registerFunction("void world_placeFloor(int x, int y, uint8 floorID)", asFunctionPtr(placeFloor));
    scriptsHandler.registerFunction("void world_placeOverlay(int x, int y, uint8 overlayID)", asFunctionPtr(placeFloor));
    scriptsHandler.registerFunction("void world_spawnMob(int x, int y, uint8 teamID)", asFunctionPtr(spawnMob));
    scriptsHandler.registerFunction("void world_spawnShell(int x, int y, uint8 teamID)", asFunctionPtr(spawnShell));
}
