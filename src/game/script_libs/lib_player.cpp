#include "script_libs.hpp"
//
#include "game/physics/team/team.hpp"
#include "game/player/player_controller.hpp"

static TeamID getTeamID() {
    Team* playerTeam = script_libs::playerController->getPlayerTeam();
    if (!playerTeam)
        script_libs::logAndThrow("Coud not get player team ID. Player team was nullptr.");
    return playerTeam->getID();
}
static TeamID getEnemyTeamID() {
    Team* enemyTeam = script_libs::world->getTeams().getTeamByName(U"enemy");
    if (!enemyTeam)
        script_libs::logAndThrow("Coud not get enemy team ID. Enemy team was nullptr.");
    return enemyTeam->getID();
}

void script_libs::registerPlayer(const ScriptsHandler& scriptsHandler) {
    scriptsHandler.registerFunction("uint8 player_getTeamID()", asFunctionPtr(getTeamID));
    scriptsHandler.registerFunction("uint8 player_getEnemyTeamID()", asFunctionPtr(getEnemyTeamID));
}
