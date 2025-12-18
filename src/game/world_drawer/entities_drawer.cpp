#include "entities_drawer.hpp"
//
#include "game/physics/team/teams_pool.hpp"
#include "game/world/world.hpp"
#include "game/physics/mob_soa.hpp"

void EntitiesDrawer::draw(const Camera& camera, const MobSoA& soa, const float tickOfset) {
    drawMobs(soa, camera, tickOfset);
}
