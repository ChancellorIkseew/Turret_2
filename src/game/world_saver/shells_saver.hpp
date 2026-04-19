#pragma once
#include "engine/io/archive.hpp"
#include "engine/io/io.hpp"
#include "game/entities/shell_manager.hpp"

void saveShells(const ShellSoA& soa, const std::filesystem::path& path) {
    std::string blob;
    archive::add(blob, soa.shellCount);
    archive::add(blob, soa.position);
    archive::add(blob, soa.velocity);
    archive::add(blob, soa.angle);
    archive::add(blob, soa.restDamage);
    archive::add(blob, soa.restLifeTime);
    archive::add(blob, soa.preset);
    archive::add(blob, soa.teamID);
    const std::string packedData = archive::pack(blob);
    io::writeFile(path / "shells.dat", packedData);
}

ShellSoA loadShells(const std::filesystem::path& path) {
    const std::string packedData = io::readFile(path / "shells.dat");
    const std::string blob = archive::unpack(packedData);
    archive::Reader reader(blob);
    ShellSoA soa;
    reader.get(soa.shellCount);
    reader.get(soa.position,     soa.shellCount);
    reader.get(soa.velocity,     soa.shellCount);
    reader.get(soa.angle,        soa.shellCount);
    reader.get(soa.restDamage,   soa.shellCount);
    reader.get(soa.restLifeTime, soa.shellCount);
    reader.get(soa.preset,       soa.shellCount);
    reader.get(soa.teamID,       soa.shellCount);
    return soa;
}
