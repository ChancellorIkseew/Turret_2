#include "ai_system.hpp"
//
#include <span>
#include "engine/assets/presets.hpp"
#include "engine/coords/math.hpp"
#include "engine/coords/transforms.hpp"
#include "game/blocks/block_map.hpp"
#include "game/blocks/schematic/schematic.hpp"
#include "game/entities/mobs_pool.hpp"
#include "game/player/player_controller.hpp"

constexpr PixelCoord NO_MOTION(0.0f, 0.0f);

static inline void updatePlayerControlled(MobSoA& soa, const Presets& presets, const size_t index, const PlayerController& playerController) {
    const PixelCoord motionVector = playerController.getMotionVector();
    if (motionVector == NO_MOTION) {
        soa.velocity[index] = NO_MOTION;
        return;
    } 
    AngleRad motionAngle = t1::atan(motionVector);// needs refactoring
    soa.angle[index] = motionAngle;//
    soa.velocity[index].x = sinf(motionAngle) * presets.getMob(soa.preset[index]).maxSpeed;//
    soa.velocity[index].y = cosf(motionAngle) * presets.getMob(soa.preset[index]).maxSpeed;//
}

static inline void updateBasic(MobSoA& soa, const Presets& presets, const size_t index) {
    const auto& aiData = soa.motionData[index];
    if (t1::areCloserRect(aiData.target, soa.position[index], presets.getMob(soa.preset[index]).maxSpeed))
        soa.velocity[index] = NO_MOTION;
    else {
        AngleRad motionAngle = t1::atan(aiData.target - soa.position[index]);//
        soa.angle[index] = motionAngle;//
        soa.velocity[index].x = sinf(motionAngle) * presets.getMob(soa.preset[index]).maxSpeed;//
        soa.velocity[index].y = cosf(motionAngle) * presets.getMob(soa.preset[index]).maxSpeed;//
    }
}

static inline void updateBuilder(MobSoA& soa, const Presets& presets, const size_t index, Schematic& schematic, BlockMap& blocks) {
    auto& aiData = soa.motionData[index];
    const PixelCoord mobPosition = soa.position[index];
    const auto closestProgress = blocks.getClosestInProgress(mobPosition);

    if (closestProgress.has_value())
        aiData.target = closestProgress->center;
    else {
        if (Blueprint* targetBlueprint = schematic.getClosest(mobPosition))
            aiData.target = targetBlueprint->center;
        else
            aiData.target = mobPosition;
    }
    
    if (t1::areCloserRect(aiData.target, mobPosition, 64.f))
        soa.velocity[index] = NO_MOTION;
    else {
        const AngleRad motionAngle = t1::atan(aiData.target - mobPosition);
        soa.angle[index] = motionAngle;
        soa.velocity[index].x = sinf(motionAngle) * presets.getMob(soa.preset[index]).maxSpeed;
        soa.velocity[index].y = cosf(motionAngle) * presets.getMob(soa.preset[index]).maxSpeed;
    }
}

void ai::updateMovingAI(MobSoA& soa, const Presets& presets, const PlayerController& playerController, Schematic& schematic, BlockMap& blocks) {
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        switch (soa.motionData[i].aiType) {
        case MovingAI::player_controlled:
            updatePlayerControlled(soa, presets, i, playerController);
            break;
        case MovingAI::basic:
            updateBasic(soa, presets, i);
            break;
        case MovingAI::builder:
            updateBuilder(soa, presets, i, schematic, blocks);
            break;
        }
    }
}
