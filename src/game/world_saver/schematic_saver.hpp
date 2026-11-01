#pragma once
#include "engine/io/archive.hpp"
#include "engine/io/io.hpp"
#include "game/blocks/schematic/schematic.hpp"

inline void saveSchematic(const Schematic& schematic, const std::filesystem::path& path) {
    std::string blob;
    const auto& blueprints = schematic.getBlueprints();
    archive::add(blob, blueprints.size());
    archive::add(blob, blueprints);
    const std::string packedData = archive::pack(blob);
    io::writeFile(path / "schematic.dat", packedData);
}

inline void loadSchematic(Schematic& schematic, const std::filesystem::path& path) {
    const std::string packedData = io::readFile(path / "schematic.dat");
    const std::string blob = archive::unpack(packedData);
    archive::Reader reader(blob);
    size_t count;
    reader.get(count);
    std::vector<Blueprint> blueprints;
    reader.get(blueprints, count);
    schematic.getBlueprints() = std::move(blueprints);
}
