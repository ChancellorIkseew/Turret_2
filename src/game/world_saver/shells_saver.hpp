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
    const std::string blobOwner = archive::unpack(packedData);
    std::string_view blob = blobOwner;
    ShellSoA soa;
    archive::get(blob, soa.shellCount);
    archive::get(blob, soa.position,     soa.shellCount);
    archive::get(blob, soa.velocity,     soa.shellCount);
    archive::get(blob, soa.angle,        soa.shellCount);
    archive::get(blob, soa.restDamage,   soa.shellCount);
    archive::get(blob, soa.restLifeTime, soa.shellCount);
    archive::get(blob, soa.preset,       soa.shellCount);
    archive::get(blob, soa.teamID,       soa.shellCount);
    return soa;
}
