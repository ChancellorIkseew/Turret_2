#include "entities_drawer.hpp"
//
#include "game/physics/team/teams_pool.hpp"
#include "game/world/world.hpp"
#include "game/physics/mobs_system.hpp"

void EntitiesDrawer::draw(const Camera& camera, const MobSoA& soa, const float tickOfset) {
    mobs::drawMobs(soa, camera, tickOfset);
}
