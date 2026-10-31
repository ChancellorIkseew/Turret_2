#pragma once
#include "engine/io/archive.hpp"
#include "engine/io/io.hpp"
#include "game/entities/turrets_pool.hpp"

void saveTurrets(const TurretSoA& soa, const std::filesystem::path& path) {
    std::string blob;
    archive::add(blob, soa.turretCount);
    archive::add(blob, soa.masterTile);
    archive::add(blob, soa.position);
    archive::add(blob, soa.angle);
    archive::add(blob, soa.preset);
    archive::add(blob, soa.teamID);
    archive::add(blob, soa.shootingData);
    archive::add(blob, soa.restReloadTime);
    archive::add(blob, soa.currentBarrel);
    archive::add(blob, soa.currentRecoil);
    archive::add(blob, soa.ammo);
    const std::string packedData = archive::pack(blob);
    io::writeFile(path / "turrets.dat", packedData);
}

TurretSoA loadTurrets(const std::filesystem::path& path) {
    const std::string packedData = io::readFile(path / "turrets.dat");
    const std::string blob = archive::unpack(packedData);
    archive::Reader reader(blob);
    TurretSoA soa;
    reader.get(soa.turretCount);
    reader.get(soa.masterTile,     soa.turretCount);
    reader.get(soa.position,       soa.turretCount);
    reader.get(soa.angle,          soa.turretCount);
    reader.get(soa.preset,         soa.turretCount);
    reader.get(soa.teamID,         soa.turretCount);
    reader.get(soa.shootingData,   soa.turretCount);
    reader.get(soa.restReloadTime, soa.turretCount);
    reader.get(soa.currentBarrel,  soa.turretCount);
    reader.get(soa.currentRecoil,  soa.turretCount);
    reader.get(soa.ammo,           soa.turretCount);
    return soa;
}
