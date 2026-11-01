#include "script_libs.hpp"
//
#include "game/common/team.hpp"
#include "game/player/player_controller.hpp"
#include "game/world/world.hpp"

static TeamID getTeamID() {
    return script_libs::playerController->getPlayerTeamID();
}
static TeamID getEnemyTeamID() {
    const TeamID enemyTeamID = script_libs::playerController->getPlayerTeamID() + 1;
    if (!script_libs::world->getTeams().containsID(enemyTeamID))
        script_libs::logAndThrow("Coud not get enemy team ID. Enemy team does not exist.");
    return enemyTeamID;
}

void script_libs::registerPlayer(const ScriptsHandler& scriptsHandler) {
    scriptsHandler.registerFunction("uint8 player_getTeamID()", asFunctionPtr(getTeamID));
    scriptsHandler.registerFunction("uint8 player_getEnemyTeamID()", asFunctionPtr(getEnemyTeamID));
}
