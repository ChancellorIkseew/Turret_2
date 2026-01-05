#include "mob_manager.hpp"
//
#include "engine/debug/logger.hpp"

constexpr MobID INVALID_MOB_ID = IDManager<MobID>::INVALID_ID;
static debug::Logger logger("mob_manager");

void MobManager::fillIndexes() { soaIndexByMobID.fill(INVALID_MOB_ID); }

void MobManager::reserve(const size_t capacity) {
    soa.id.reserve(capacity);
    soa.position.reserve(capacity);
    soa.velocity.reserve(capacity);
    soa.angle.reserve(capacity);
    soa.teamID.reserve(capacity);
    soa.preset.reserve(capacity);
    soa.hitbox.reserve(capacity);
    soa.motionData.reserve(capacity);
    soa.shootingData.reserve(capacity);
    soa.restReloadTime.reserve(capacity);
    soa.currentBarrel.reserve(capacity);
    soa.turretAngle.reserve(capacity);
    soa.chassisFrame.reserve(capacity);
    soa.turretFrame.reserve(capacity);
}

MobID MobManager::addMob(
    const csp::centralized_ptr<MobPreset>& preset,
    const PixelCoord position,
    const AngleRad angle,
    const Health health,
    const TeamID teamID,
    const MotionData motionData,
    const ShootingData shootingData,
    const TickCount restReloadTime,
    const AngleRad turretAngle) {
    const MobID mobID = idManager.getNext();

    if (mobID == INVALID_MOB_ID) {
        logger.warning() << "Could not add mob to SoA. No more avaliable free MobID.";
        return INVALID_MOB_ID;
    }

    soa.id.push_back(mobID);
    soa.position.push_back(position);
    soa.velocity.push_back(PixelCoord(0, 0));
    soa.angle.push_back(angle);
    soa.health.push_back(health);
    soa.teamID.push_back(teamID);
    soa.preset.push_back(preset);
    soa.hitbox.push_back(Hitbox(position, preset->hitboxRadius));
    soa.motionData.push_back(motionData);
    soa.shootingData.push_back(shootingData);
    soa.restReloadTime.push_back(restReloadTime);
    soa.currentBarrel.push_back(0);
    soa.turretAngle.push_back(turretAngle);
    soa.chassisFrame.push_back(0);
    soa.turretFrame.push_back(0);

    const size_t last = soa.id.size() - 1;
    soaIndexByMobID[last] = mobID;
    ++soa.mobCount;
    return mobID;
}

void MobManager::removeMob(const size_t index) {
    const size_t last = soa.id.size() - 1;
    soaIndexByMobID[index] = soaIndexByMobID[last];
    soaIndexByMobID[last] = INVALID_MOB_ID;
    idManager.setFree(soa.id[index]);
    --soa.mobCount;

    if (index != last) {
        soa.id[index] = std::move(soa.id[last]);
        soa.position[index] = std::move(soa.position[last]);
        soa.velocity[index] = std::move(soa.velocity[last]);
        soa.angle[index] = std::move(soa.angle[last]);
        soa.health[index] = std::move(soa.health[last]);
        soa.teamID[index] = std::move(soa.teamID[last]);
        soa.preset[index] = std::move(soa.preset[last]);
        soa.hitbox[index] = std::move(soa.hitbox[last]);
        soa.motionData[index] = std::move(soa.motionData[last]);
        soa.shootingData[index] = std::move(soa.shootingData[last]);
        soa.restReloadTime[index] = std::move(soa.restReloadTime[last]);
        soa.currentBarrel[index] = std::move(soa.currentBarrel[last]);
        soa.turretAngle[index] = std::move(soa.turretAngle[last]);
        soa.chassisFrame[index] = std::move(soa.chassisFrame[last]);
        soa.turretFrame[index] = std::move(soa.turretFrame[last]);
    }

    soa.id.pop_back();
    soa.position.pop_back();
    soa.velocity.pop_back();
    soa.angle.pop_back();
    soa.health.pop_back();
    soa.teamID.pop_back();
    soa.preset.pop_back();
    soa.hitbox.pop_back();
    soa.motionData.pop_back();
    soa.shootingData.pop_back();
    soa.restReloadTime.pop_back();
    soa.currentBarrel.pop_back();
    soa.turretAngle.pop_back();
    soa.chassisFrame.pop_back();
    soa.turretFrame.pop_back();
}
