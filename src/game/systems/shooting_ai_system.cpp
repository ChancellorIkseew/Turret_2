#include "ai_system.hpp"
//
#include <algorithm>
#include "engine/assets/presets.hpp"
#include "engine/coords/math.hpp"
#include "engine/coords/transforms.hpp"
#include "game/player/player_controller.hpp"
#include "game/entities/mobs_pool.hpp"
#include "turret_components.hpp"
#include "game/blocks/block_map.hpp"

static inline void updatePlayerControlled(TurretComponents& soa, const Presets& presets,
    const size_t index, const PlayerController& playerController) {
    soa.shootingData[index].isShooting = playerController.shootingActive();
    soa.shootingData[index].target = playerController.getAimCoord();
}

struct Aim {
    float squareDistance;
    PixelCoord position;
    //
    static constexpr bool closest(const Aim first, const Aim second) {
        return first.squareDistance < second.squareDistance;
    }
};

static inline void updateBasic(TurretComponents& soa, const MobSoA& mobs, const BlockMap& blocks, const Presets& presets, const size_t index) {
    const float range = presets.getTurret(soa.preset[index]).range;
    const float sqRange = t1::pow2f(range);
    const PixelCoord position = soa.position[index];
    const TeamID teamID = soa.teamID[index];
    soa.shootingData[index].isShooting = false;

    // aim to mob
    static std::vector<Aim> aims;
    for (size_t i = 0; i < mobs.mobCount; ++i) {
        if (teamID == mobs.teamID[i])
            continue;
        const float sqDistance = t1::squareDistance(position, mobs.position[i]);
        if (sqDistance < sqRange)
            aims.emplace_back(sqDistance, mobs.position[i]);
    }

    //aim to block
    const int tileRange = t1::tile(range);
    const TileCoord tilePosition = t1::tile(position);
    const TileCoord mapSize = blocks.getSize();
    const int startX = std::max(0, tilePosition.x - tileRange);
    const int startY = std::max(0, tilePosition.y - tileRange);
    const int endX = std::min(mapSize.x, tilePosition.x + tileRange);
    const int endY = std::min(mapSize.y, tilePosition.y + tileRange);
    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY; ++y) {
            const BlockTile& blockTile = blocks.at(x, y);
            if (blockTile.type == BlockType::air)
                continue;
            if (blockTile.teamID == teamID)
                continue;
            const PixelCoord blockCenter = t1::tileCenter({ x, y });
            const float sqDistance = t1::squareDistance(position, blockCenter);
            if (sqDistance < sqRange)
                aims.emplace_back(sqDistance, blockCenter);
        }
    }

    if (!aims.empty()) {
        const auto aim = std::min_element(aims.begin(), aims.end(), Aim::closest);
        soa.shootingData[index].isShooting = true;
        soa.shootingData[index].target = aim->position;
        aims.clear();
        return;
    }
}

void ai::updateShootingAI(TurretComponents& soa, const MobSoA& mobs, const BlockMap& blocks,
    const Presets& presets, const PlayerController& playerController) {
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        switch (soa.shootingData[i].aiType) {
        case ShootingAI::player_controlled:
            updatePlayerControlled(soa, presets, i, playerController);
            break;
        case ShootingAI::basic:
            updateBasic(soa, mobs, blocks, presets, i);
            break;
        }
    }
}
