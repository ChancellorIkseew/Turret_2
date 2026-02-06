#include "mob_manager.hpp"
//
#include "engine/debug/logger.hpp"

constexpr MobIndex INVALID_MOB_ID = IDManager<MobID>::INVALID_ID;
constexpr MobIndex INVALID_MOB_INDEX = INVALID_MOB_ID;
static debug::Logger logger("mob_manager");

void MobManager::fillIndexes() { soaIndexByMobID.fill(INVALID_MOB_INDEX); }

void MobManager::reserve(const size_t capacity) {
    soa.id.reserve(capacity);
    soa.position.reserve(capacity);
    soa.velocity.reserve(capacity);
    soa.angle.reserve(capacity);
    soa.teamID.reserve(capacity);
    soa.preset.reserve(capacity);
    soa.hitboxRadius.reserve(capacity);
    soa.motionData.reserve(capacity);
    soa.shootingData.reserve(capacity);
    soa.restReloadTime.reserve(capacity);
    soa.currentBarrel.reserve(capacity);
    soa.turretAngle.reserve(capacity);
    soa.chassisFrame.reserve(capacity);
    soa.turretFrame.reserve(capacity);
}

MobID MobManager::addMob(
    const MobPresetID preset,
    const PixelCoord position,
    const AngleRad angle,
    const Health health,
    const TeamID teamID,
    const float hitboxRadius,
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
    soa.hitboxRadius.push_back(hitboxRadius);
    soa.motionData.push_back(motionData);
    soa.shootingData.push_back(shootingData);
    soa.restReloadTime.push_back(restReloadTime);
    soa.currentBarrel.push_back(0);
    soa.turretAngle.push_back(turretAngle);
    soa.chassisFrame.push_back(0);
    soa.turretFrame.push_back(0);

    soaIndexByMobID[mobID] = static_cast<MobIndex>(soa.mobCount);
    ++soa.mobCount;
    return mobID;
}

void MobManager::removeMob(const size_t targetIndex) {
    const size_t lastIndex = --soa.mobCount;
    idManager.setFree(soa.id[targetIndex]);
    soaIndexByMobID[soa.id[lastIndex]] = static_cast<MobIndex>(targetIndex);
    soaIndexByMobID[soa.id[targetIndex]] = INVALID_MOB_INDEX;

    if (targetIndex != lastIndex) {
        soa.id[targetIndex] = std::move(soa.id[lastIndex]);
        soa.position[targetIndex] = std::move(soa.position[lastIndex]);
        soa.velocity[targetIndex] = std::move(soa.velocity[lastIndex]);
        soa.angle[targetIndex] = std::move(soa.angle[lastIndex]);
        soa.health[targetIndex] = std::move(soa.health[lastIndex]);
        soa.teamID[targetIndex] = std::move(soa.teamID[lastIndex]);
        soa.preset[targetIndex] = std::move(soa.preset[lastIndex]);
        soa.hitboxRadius[targetIndex] = std::move(soa.hitboxRadius[lastIndex]);
        soa.motionData[targetIndex] = std::move(soa.motionData[lastIndex]);
        soa.shootingData[targetIndex] = std::move(soa.shootingData[lastIndex]);
        soa.restReloadTime[targetIndex] = std::move(soa.restReloadTime[lastIndex]);
        soa.currentBarrel[targetIndex] = std::move(soa.currentBarrel[lastIndex]);
        soa.turretAngle[targetIndex] = std::move(soa.turretAngle[lastIndex]);
        soa.chassisFrame[targetIndex] = std::move(soa.chassisFrame[lastIndex]);
        soa.turretFrame[targetIndex] = std::move(soa.turretFrame[lastIndex]);
    }

    soa.id.pop_back();
    soa.position.pop_back();
    soa.velocity.pop_back();
    soa.angle.pop_back();
    soa.health.pop_back();
    soa.teamID.pop_back();
    soa.preset.pop_back();
    soa.hitboxRadius.pop_back();
    soa.motionData.pop_back();
    soa.shootingData.pop_back();
    soa.restReloadTime.pop_back();
    soa.currentBarrel.pop_back();
    soa.turretAngle.pop_back();
    soa.chassisFrame.pop_back();
    soa.turretFrame.pop_back();
}
