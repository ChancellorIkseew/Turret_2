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
#include "game/entities/particles_pool.hpp"

constexpr uint32_t ALPHA = 0xFF'FF'FF'A0;
constexpr uint32_t BUILD_COLOR = cl::BEIGE & ALPHA;
constexpr uint32_t BREAK_COLOR = cl::RED   & ALPHA;
constexpr uint32_t FADING = 3;
constexpr TickCount LIFE_TICKS = 50;

static void onBlockPlace(SoundQueue& sounds, ParticlesPool& particles, PixelCoord position, int size) {
    sounds.pushSound("block_place", position);
    particles.addParticle(position, t1::pixel(size, size), 0.f, 0.f, BUILD_COLOR, FADING, LIFE_TICKS, PType::shard);
}

static void onBlockBreak(SoundQueue& sounds, ParticlesPool& particles, PixelCoord position, int size) {
    sounds.pushSound("block_break", position);
    particles.addParticle(position, t1::pixel(size, size), 0.f, 0.f, BREAK_COLOR, FADING, LIFE_TICKS, PType::shard);
}

void construction::buildBlueprints(MobSoA& soa, const Presets& presets, Schematic& schematic,
    BlockMap& blocks, BuildBeamsPool& buildBeams, TeamsPool& teams, SoundQueue& sounds, ParticlesPool& particles) {
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
                onBlockPlace(sounds, particles, closestInProgress->center, blockSize);
            if (result == Result::demolish_complite)
                onBlockBreak(sounds, particles, closestInProgress->center, blockSize);
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
