#include "construction_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/audio/sound_queue.hpp"
#include "engine/coords/transforms.hpp"
#include "game/blocks/block_map.hpp"
#include "game/blocks/schematic/schematic.hpp"
#include "game/common/teams_pool.hpp"
#include "game/entities/build_beams.hpp"
#include "game/entities/mobs_pool.hpp"

static void onBlockPlace(SoundQueue& sounds, PixelCoord position) {
    sounds.pushSound("block_place", position);
}

static void onBlockBreak(SoundQueue& sounds, PixelCoord position) {
    sounds.pushSound("block_break", position);
}

void construction::buildBlueprints(MobSoA& soa, const Presets& presets, Schematic& schematic,
    BlockMap& blocks, BuildBeamsPool& buildBeams, TeamsPool& teams, SoundQueue& sounds) {
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
            const int blockSize = blocks.at(targetTile).block->size;
            soa.angle[i] = t1::atan(closestInProgress->center - position);

            using Result = BlockMap::BuildResult;
            Inventory& teamInventory = teams.getTeamByID(soa.teamID[i])->getInventory();
            const Result result = blocks.build(targetTile, soa.teamID[i], mobPreset.buildSpeed, presets, teamInventory);

            const uint32_t color = (result <= Result::build) ? cl::BEIGE : cl::RED;
            buildBeams.addBeam(position, targetTile, blockSize, color);

            if (result == Result::build_complite)
                onBlockPlace(sounds, closestInProgress->center/*particles, blockSize*/);
            if (result == Result::demolish_complite)
                onBlockBreak(sounds, closestInProgress->center/*particles, blockSize*/);
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
