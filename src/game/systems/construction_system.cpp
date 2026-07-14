#include "construction_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/coords/transforms.hpp"
#include "game/blocks/block_map.hpp"
#include "game/entities/mobs_pool.hpp"
#include "game/frontend/build_tools/blueprint.hpp"
#include "game/built_in_scripts/built_in_scripts.hpp"

struct Aim {
    float squareDistance;
    PixelCoord position;
    //
    static constexpr bool closest(const Aim first, const Aim second) {
        return first.squareDistance < second.squareDistance;
    }
};

void construction::buildBlueprints(MobSoA& soa, const Presets& presets, Blueprints& blueprints, BuiltInScripts& scripts, BlockMap& blocks) {
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
            const auto aim = std::min_element(aims.begin(), aims.end(), Aim::closest);
            const TileCoord targetTile = t1::tile(aim->position);
            InProgress* block = static_cast<InProgress*>(blocks.at(targetTile).block.get());
            block->progress += block->action == BPAction::build ? mobPreset.buildSpeed : -mobPreset.buildSpeed;
            if (block->progressFull() && block->action == BPAction::build)
                scripts.placeBlock(block->presetID, targetTile, soa.teamID[i], block->rotation);
            if (block->progressFull() && block->action == BPAction::demolish)
                blocks.demolish(targetTile);
            soa.angle[i] = t1::atan(aim->position - position);
            aims.clear();
            continue;
        }

        Blueprint* bp = blueprints.getClosest(tilePosition);
        if (!bp)
            continue;
        const PixelCoord tileCenter = t1::tileCenter(bp->tile);
        if (t1::areCloserCircle(tileCenter, position, RANGE)) {
            blocks.demolish(bp->tile);
            scripts.placeBlockInProgress(bp->presetID, bp->tile, soa.teamID[i], bp->rotation, bp->action);
            blueprints.removeIfExists(bp->tile);
            soa.angle[i] = t1::atan(tileCenter - position);
        }
    }
}
