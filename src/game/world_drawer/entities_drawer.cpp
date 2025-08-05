#include "entities_drawer.hpp"
//
#include "game/physics/team/teams_pool.hpp"
#include "game/world/world.hpp"

EntitiesDrawer::EntitiesDrawer(const Camera& camera, const World& world) :
    camera(camera), teams(world.getTeams()) { }

void EntitiesDrawer::draw() {
    for (const auto& [_teamID, team] : teams) {
        team->drawGroundUnits(camera);
    }
    for (const auto& [_teamID, team] : teams) {
        team->drawShells(camera);
    }
    for (const auto& [_teamID, team] : teams) {
        team->drawAirUnits(camera);
    }
}
