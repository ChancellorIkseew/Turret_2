#include "script_libs.hpp"
//
#include "game/physics/team/team.hpp"
#include "game/player/player_controller.hpp"

static TeamID getTeamID() {
    Team* playerTeam = PlayerController::getPlayerTeam();
    if (!playerTeam)
        script_libs::logAndThrow("Coud not get player team ID. Player team was nullptr.");
    return playerTeam->getID();
}

void script_libs::registerPlayer(const ScriptsHandler& scriptsHandler) {
    scriptsHandler.registerFunction("uint8 player_getTeamID()", asFunctionPtr(getTeamID));
}
