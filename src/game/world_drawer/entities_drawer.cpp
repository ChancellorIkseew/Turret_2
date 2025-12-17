#include "entities_drawer.hpp"
//
#include "game/physics/team/teams_pool.hpp"
#include "game/world/world.hpp"

void EntitiesDrawer::draw(const Camera& camera, const TeamsPool& teams, const float tickOfset) {
    for (const auto& [_teamID, team] : teams) {
        team->drawGroundUnits(camera, tickOfset);
    }
    for (const auto& [_teamID, team] : teams) {
        team->drawShells(camera, tickOfset);
    }
    for (const auto& [_teamID, team] : teams) {
        team->drawAirUnits(camera, tickOfset);
    }
}
