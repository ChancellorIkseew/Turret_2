#pragma once
#include "engine/io/archive.hpp"
#include "engine/io/io.hpp"
#include "game/entities/mobs_pool.hpp"

void saveShells(const MobSoA& soa, const std::filesystem::path& path) {
    std::string blob;
    archive::add(blob, soa.mobCount);
    archive::add(blob, soa.hitboxRadius);
    archive::add(blob, soa.position);
    archive::add(blob, soa.preveousePosition);
    archive::add(blob, soa.velocity);
    archive::add(blob, soa.angle);
    archive::add(blob, soa.health);
    archive::add(blob, soa.shieldHealth);
    archive::add(blob, soa.turretPreset);
    archive::add(blob, soa.teamID);
    archive::add(blob, soa.motionData);
    archive::add(blob, soa.shootingData);
    archive::add(blob, soa.restReloadTime);
    archive::add(blob, soa.currentBarrel);
    archive::add(blob, soa.turretAngle);
    archive::add(blob, soa.currentRecoil);
    archive::add(blob, soa.chassisTick);
    archive::add(blob, soa.ammo);
    const std::string packedData = archive::pack(blob);
    io::writeFile(path / "mobs.dat", packedData);
}

MobSoA loadShells(const std::filesystem::path& path) {
    const std::string packedData = io::readFile(path / "mobs.dat");
    const std::string blob = archive::unpack(packedData);
    archive::Reader reader(blob);
    MobSoA soa;
    reader.get(soa.mobCount);
    reader.get(soa.hitboxRadius,      soa.mobCount);
    reader.get(soa.position,          soa.mobCount);
    reader.get(soa.preveousePosition, soa.mobCount);
    reader.get(soa.velocity,          soa.mobCount);
    reader.get(soa.angle,             soa.mobCount);
    reader.get(soa.health,            soa.mobCount);
    reader.get(soa.shieldHealth,      soa.mobCount);
    reader.get(soa.turretPreset,      soa.mobCount);
    reader.get(soa.teamID,            soa.mobCount);
    reader.get(soa.motionData,        soa.mobCount);
    reader.get(soa.shootingData,      soa.mobCount);
    reader.get(soa.restReloadTime,    soa.mobCount);
    reader.get(soa.currentBarrel,     soa.mobCount);
    reader.get(soa.turretAngle,       soa.mobCount);
    reader.get(soa.currentRecoil,     soa.mobCount);
    reader.get(soa.chassisTick,       soa.mobCount);
    reader.get(soa.ammo);
    return soa;
}
