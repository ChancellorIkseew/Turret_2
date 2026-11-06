#include "ai_system.hpp"
//
#include "engine/assets/presets.hpp"
#include "engine/coords/math.hpp"
#include "engine/coords/transforms.hpp"
#include "game/player/player_controller.hpp"
#include "game/entities/mobs_pool.hpp"
#include "turret_components.hpp"
#include "game/blocks/block_map.hpp"

static inline void updatePlayerControlled(TurretComponents& soa, const Presets& presets,
    const size_t index, const PlayerController& playerController) {
    soa.shootingData[index].hasTarget = playerController.isShooting();
    soa.shootingData[index].target = playerController.getAimCoord();
}

static inline void updateBasic(TurretComponents& soa, const MobSoA& mobs, const BlockMap& blocks,
    const Presets& presets, const size_t index) {
    const float shellSpeed = presets.getShell(presets.getTurret(soa.preset[index]).shell).speed;
    const float range = presets.getTurret(soa.preset[index]).range;
    const float sqRange = t1::pow2f(range);
    const PixelCoord position = soa.position[index];
    const TeamID teamID = soa.teamID[index];
    soa.shootingData[index].hasTarget = false;

    // find closest mob
    float bestMobSqDist = sqRange;
    std::optional<size_t> bestMobIndex;
    for (size_t i = 0; i < mobs.mobCount; ++i) {
        if (teamID == mobs.teamID[i])
            continue;
        const float sqDistance = t1::squareDistance(position, mobs.position[i]);
        if (sqDistance < bestMobSqDist) {
            bestMobSqDist = sqDistance;
            bestMobIndex = i;
        }
    }

    // find closest block
    float bestBlockSqDist = sqRange;
    PixelCoord bestBlockPos;
    bool hasBlockTarget = false;
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
            if (blockTile.type == BlockType::air || blockTile.teamID == teamID)
                continue;
            const PixelCoord blockCenter = t1::tileCenter({ x, y });
            const float sqDistance = t1::squareDistance(position, blockCenter);
            if (sqDistance < bestBlockSqDist) {
                bestBlockSqDist = sqDistance;
                bestBlockPos = blockCenter;
                hasBlockTarget = true;
            }
        }
    }

    // choose target + apply lead
    const bool hasMobTarget = bestMobIndex.has_value();
    if (hasMobTarget && (!hasBlockTarget || bestMobSqDist <= bestBlockSqDist)) {
        const PixelCoord actualVelocity = mobs.position[*bestMobIndex] - mobs.preveousePosition[*bestMobIndex];
        const float timeToHit = std::sqrt(bestMobSqDist) / shellSpeed;
        soa.shootingData[index].hasTarget = true;
        soa.shootingData[index].target = mobs.position[*bestMobIndex] + actualVelocity * timeToHit;
    }
    else if (hasBlockTarget) {
        soa.shootingData[index].hasTarget = true;
        soa.shootingData[index].target = bestBlockPos;
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
            if (soa.ammo[i] > 0) // mb disolve
                updateBasic(soa, mobs, blocks, presets, i);
            break;
        }
    }
}
