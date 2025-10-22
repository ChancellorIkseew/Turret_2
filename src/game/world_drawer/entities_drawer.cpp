#include "entities_drawer.hpp"
//
#include "game/physics/team/teams_pool.hpp"
#include "game/world/world.hpp"

EntitiesDrawer::EntitiesDrawer(const Camera& camera, const World& world) :
    camera(camera), teams(world.getTeams()) { }

void EntitiesDrawer::draw(const uint64_t deltaT) {
    for (const auto& [_teamID, team] : teams) {
        team->drawGroundUnits(camera, deltaT);
    }
    for (const auto& [_teamID, team] : teams) {
        team->drawShells(camera, deltaT);
    }
    for (const auto& [_teamID, team] : teams) {
        team->drawAirUnits(camera, deltaT);
    }
}
