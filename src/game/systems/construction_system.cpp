#include "construction_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/coords/transforms.hpp"
#include "game/entities/mobs_pool.hpp"
#include "game/frontend/build_tools/blueprint.hpp"
#include "game/built_in_scripts/built_in_scripts.hpp"

void construction::buildBlueprints(MobSoA& soa, const Presets& presets, Blueprints& blueprints, BuiltInScripts& scripts) {
    if (blueprints.empty())
        return;
    for (size_t i = 0; i < soa.mobCount; ++i) {
        const auto& mobPreset = presets.getMob(soa.preset[i]);
        if (!mobPreset.canBuild)
            continue;
        Blueprint* bp = blueprints.getClosest(t1::tile(soa.position[i]));
        if (!bp)
            continue;
        const PixelCoord tileCenter = t1::tileCenter(bp->tile);
        if (t1::areCloserCircle(tileCenter, soa.position[i], 128.f)) {
            if (bp->progress < 100) {
                bp->progress += mobPreset.buildSpeed;
                soa.angle[i] = t1::atan(tileCenter - soa.position[i]);
            } 
            else {
                scripts.placeBlock(bp->presetID, bp->tile, soa.teamID[i], bp->rotation);
                blueprints.removeIfExists(bp->tile);
            }
            continue;
        }
    }
}
