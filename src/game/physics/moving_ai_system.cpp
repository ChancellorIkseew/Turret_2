#include "ai_system.hpp"
//
#include "mob_manager.hpp"
#include "game/player/player_controller.hpp"

constexpr PixelCoord NO_MOTION(0.0f, 0.0f);

static inline float atan2f(const PixelCoord p) noexcept { return atan2f(p.x, p.y); }

static inline void updatePlayerControlled(MobSoA& soa, const size_t index, const PlayerController& playerController) {
    const PixelCoord motionVector = playerController.getMotionVector();
    if (motionVector == NO_MOTION) {
        soa.velocity[index] = NO_MOTION;
        return;
    } 
    AngleRad motionAngle = atan2f(motionVector);// needs refactoring
    soa.angle[index] = motionAngle;//
    soa.velocity[index].x = sinf(motionAngle) * soa.preset[index]->maxSpeed;//
    soa.velocity[index].y = cosf(motionAngle) * soa.preset[index]->maxSpeed;//
}

static inline void updateBasic(MobSoA& soa, const size_t index) {
    const auto& aiData = soa.motionData[index];
    if (t1::areCloser(aiData.target, soa.position[index], soa.preset[index]->maxSpeed))
        soa.velocity[index] = NO_MOTION;
    else {
        AngleRad motionAngle = atan2f(aiData.target - soa.position[index]);//
        soa.angle[index] = motionAngle;//
        soa.velocity[index].x = sinf(motionAngle) * soa.preset[index]->maxSpeed;//
        soa.velocity[index].y = cosf(motionAngle) * soa.preset[index]->maxSpeed;//
    }
}

void ai::updateMovingAI(MobSoA& soa, const PlayerController& playerController) {
    const size_t mobCount = soa.id.size();
    for (size_t i = 0; i < mobCount; ++i) {
        switch (soa.motionData[i].aiType) {
        case MovingAI::player_controlled:
            updatePlayerControlled(soa, i, playerController);
            break;
        case MovingAI::basic:
            updateBasic(soa, i);
            break;
        }
    }
}
