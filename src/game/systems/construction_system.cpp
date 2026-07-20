#include "construction_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/coords/transforms.hpp"
#include "game/blocks/block_map.hpp"
#include "game/blocks/schematic/schematic.hpp"
#include "game/entities/build_beams.hpp"
#include "game/entities/mobs_pool.hpp"
#include "game/built_in_scripts/built_in_scripts.hpp"

void construction::buildBlueprints(MobSoA& soa, const Presets& presets, Schematic& schematic,
    BlockMap& blocks, BuildBeamsPool& buildBeams) {
    for (size_t i = 0; i < soa.mobCount; ++i) {
        const auto& mobPreset = presets.getMob(soa.preset[i]);
        if (!mobPreset.canBuild)
            continue;

        const PixelCoord position = soa.position[i];
        constexpr float RANGE = 128.f;
        constexpr float SQ_RANGE = t1::pow2f(RANGE);
        const auto closestInProgress = blocks.getClosestInProgress(position);

        if (closestInProgress.has_value() && closestInProgress->squareDistance < SQ_RANGE) {
            const TileCoord targetTile = closestInProgress->masterTile;
            soa.angle[i] = t1::atan(closestInProgress->center - position);

            const InProgress* block = static_cast<const InProgress*>(blocks.at(targetTile).block.get());
            const uint32_t color = (block->action == BPAction::build) ? 0xFA'DC'86'00 : 0x84'34'34'00;

            blocks.build(targetTile, soa.teamID[i], mobPreset.buildSpeed, presets);
            buildBeams.addBeam(position, targetTile, block->size, color);
            continue;
        }

        Blueprint* bp = schematic.getClosest(position);
        if (!bp)
            continue;
        if (t1::areCloserCircle(bp->center, position, RANGE)) {
            soa.angle[i] = t1::atan(bp->center - position);
            blocks.applyBlueprint(*bp, soa.teamID[i], presets);
            schematic.removeIfExists(bp->tile);
        }
    }
}
