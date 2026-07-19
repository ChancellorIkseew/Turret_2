#include "construction_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/coords/transforms.hpp"
#include "game/blocks/block_map.hpp"
#include "game/entities/build_beams.hpp"
#include "game/entities/mobs_pool.hpp"
#include "game/frontend/build_tools/blueprint.hpp"
#include "game/built_in_scripts/built_in_scripts.hpp"

void construction::buildBlueprints(MobSoA& soa, const Presets& presets, Blueprints& blueprints,
    BlockMap& blocks, BuildBeamsPool& buildBeams) {
    for (size_t i = 0; i < soa.mobCount; ++i) {
        const auto& mobPreset = presets.getMob(soa.preset[i]);
        if (!mobPreset.canBuild)
            continue;

        static std::vector<Aim> aims;
        const PixelCoord position = soa.position[i];
        constexpr float RANGE = 128.f; //temporary
        constexpr float SQ_RANGE = t1::pow2f(RANGE); //temporary
        constexpr int TILE_RANGE = t1::tile(RANGE); //temporary
        const TileCoord tilePosition = t1::tile(position);
        const TileCoord mapSize = blocks.getSize();
        const int startX = std::max(0, tilePosition.x - TILE_RANGE);
        const int startY = std::max(0, tilePosition.y - TILE_RANGE);
        const int endX = std::min(mapSize.x, tilePosition.x + TILE_RANGE);
        const int endY = std::min(mapSize.y, tilePosition.y + TILE_RANGE);
        for (int x = startX; x < endX; ++x) {
            for (int y = startY; y < endY; ++y) {
                const BlockTile& blockTile = blocks.at(x, y);
                if (blockTile.type != BlockType::in_progress)
                    continue;
                const PixelCoord blockCenter = t1::tileCenter({ x, y });
                const float sqDistance = t1::squareDistance(position, blockCenter);
                if (sqDistance < SQ_RANGE)
                    aims.emplace_back(sqDistance, blockCenter);
            }
        }

        if (!aims.empty()) {
            const auto aim = std::ranges::min_element(aims, Aim::closest);
            const TileCoord targetTile = t1::tile(aim->position);
            soa.angle[i] = t1::atan(aim->position - position);
            const BPAction action = static_cast<InProgress*>(blocks.at(targetTile).block.get())->action; //temp
            const uint32_t color = (action == BPAction::build) ? 0xFA'DC'86'00 : 0x84'34'34'00;
            const int blockSize = blocks.at(targetTile).block->size;
            blocks.build(targetTile, soa.teamID[i], mobPreset.buildSpeed, presets);
            buildBeams.addBeam(position, targetTile, blockSize, color);
            aims.clear();
            continue;
        }

        Blueprint* bp = blueprints.getClosest(position);
        if (!bp)
            continue;
        if (t1::areCloserCircle(bp->center, position, RANGE)) {
            soa.angle[i] = t1::atan(bp->center - position);
            blocks.applyBlueprint(*bp, soa.teamID[i], presets);
            blueprints.removeIfExists(bp->tile);
        }
    }
}
