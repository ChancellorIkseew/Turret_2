#include "ai_system.hpp"
//
#include <algorithm>
#include "engine/assets/presets.hpp"
#include "engine/coords/math.hpp"
#include "engine/coords/transforms.hpp"
#include "game/player/player_controller.hpp"
#include "mob_manager.hpp"
#include "turret_components.hpp"

static inline void updatePlayerControlled(TurretComponents& soa, const Presets& presets,
    const size_t index, const PlayerController& playerController) {
    soa.shootingData[index].isShooting = playerController.shootingActive();
    soa.shootingData[index].target = playerController.getAimCoord();
}

static inline void updateBasic(TurretComponents& soa, const MobSoA& mobs, const BlockManager& blocks, const Presets& presets, const size_t index) {
    const float range = presets.getTurret(soa.preset[index]).range;
    const PixelCoord position = soa.position[index];
    const TeamID teamID = soa.teamID[index];
    soa.shootingData[index].isShooting = false;

    // aim to mob
    for (size_t i = 0; i < mobs.mobCount; ++i) {
        if (teamID == mobs.teamID[i])
            continue;
        if (t1::areCloserCircle(position, mobs.position[i], range)) {
            soa.shootingData[index].isShooting = true;
            soa.shootingData[index].target = mobs.position[i];
            return;
        }
    }

    //aim to block
    const auto& blockSoa = blocks.getCommonSoa();
    const int tileRange = t1::tile(range);
    const TileCoord tilePosition = t1::tile(position);
    const TileCoord mapSize = blocks.getSize();
    const int startX = std::max(0, tilePosition.x - tileRange);
    const int startY = std::max(0, tilePosition.y - tileRange);
    const int endX = std::min(mapSize.x, tilePosition.x + tileRange);
    const int endY = std::min(mapSize.y, tilePosition.y + tileRange);
    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY; ++y) {
            const PixelCoord blockCenter = t1::tileCenter({ x, y });
            if (!t1::areCloserCircle(position, blockCenter, range))
                continue;
            if (blockSoa.archetype[blocks.at(x, y)] == BlockArchetype::air)
                continue;
            soa.shootingData[index].isShooting = true;
            soa.shootingData[index].target = blockCenter;
        }
    }
}

void ai::updateShootingAI(TurretComponents& soa, const MobSoA& mobs, const BlockManager& blocks,
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
