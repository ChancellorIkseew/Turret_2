#include "turret_manager.hpp"

void TurretManager::reserve(const size_t capacity) {
    soa.position.reserve(capacity);
    soa.angle.reserve(capacity);
    soa.teamID.reserve(capacity);
    soa.preset.reserve(capacity);
    soa.shootingData.reserve(capacity);
    soa.restReloadTime.reserve(capacity);
    soa.currentBarrel.reserve(capacity);
    soa.turretFrame.reserve(capacity);
}

void TurretManager::addTurret(
    const TurretPresetID preset,
    const PixelCoord position,
    const AngleRad angle,
    const TeamID teamID,
    const ShootingData shootingData,
    const TickCount restReloadTime) {

    soa.position.push_back(position);
    soa.angle.push_back(angle);
    soa.teamID.push_back(teamID);
    soa.preset.push_back(preset);
    soa.shootingData.push_back(shootingData);
    soa.restReloadTime.push_back(restReloadTime);
    soa.currentBarrel.push_back(0);
    soa.turretFrame.push_back(0);
    ++soa.turretCount;
}

void TurretManager::removeTurret(const size_t targetIndex) {
    const size_t last = --soa.turretCount;

    if (targetIndex != last) {
        soa.position[targetIndex] = std::move(soa.position[last]);
        soa.angle[targetIndex] = std::move(soa.angle[last]);
        soa.teamID[targetIndex] = std::move(soa.teamID[last]);
        soa.preset[targetIndex] = std::move(soa.preset[last]);
        soa.shootingData[targetIndex] = std::move(soa.shootingData[last]);
        soa.restReloadTime[targetIndex] = std::move(soa.restReloadTime[last]);
        soa.currentBarrel[targetIndex] = std::move(soa.currentBarrel[last]);
        soa.turretFrame[targetIndex] = std::move(soa.turretFrame[last]);
    }

    soa.position.pop_back();
    soa.angle.pop_back();
    soa.teamID.pop_back();
    soa.preset.pop_back();
    soa.shootingData.pop_back();
    soa.restReloadTime.pop_back();
    soa.currentBarrel.pop_back();
    soa.turretFrame.pop_back();
}
