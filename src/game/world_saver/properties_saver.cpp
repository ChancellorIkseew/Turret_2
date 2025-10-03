#include "properties_saver.hpp"
//
#include "engine/debug/logger.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "game/generation/generation.hpp"
#include "game/world/map_content.hpp"

static debug::Logger logger("properties_saver");

void PropertiesSaver::saveContentIndexes(const ContentIndexes& indexes, const std::filesystem::path& path) {
    tin::Data data;
    for (const auto& [name, index] : indexes.floorTypes) {
        data.emplace(name, std::to_string(index));
    }
    for (const auto& [name, index] : indexes.overlayTypes) {
        data.emplace(name, std::to_string(index));
    }
    for (const auto& [name, index] : indexes.blockTypes) {
        data.emplace(name, std::to_string(index));
    }
    tin::write(path / "content_indexes.tin", data);
}

ContentIndexes PropertiesSaver::loadContentIndexes(const std::filesystem::path& path) {
    tin::Data data = tin::read(path/ "content_indexes.tin");
    ContentIndexes indexes;
    for (const auto& [name, index] : data) {
        if (name.find("floor_") != std::string::npos)
            indexes.floorTypes.emplace(name, validator::toUint8(index).value_or(0U));
        else if (name.find("overlay_") != std::string::npos)
            indexes.overlayTypes.emplace(name, validator::toUint8(index).value_or(0U));
        else if (name.find("block_") != std::string::npos)
            indexes.blockTypes.emplace(name, validator::toUint16(index).value_or(0U));
    }
    return indexes;
}

void PropertiesSaver::saveOverlayPreset(const OverlayPresets& overlayPresets, const std::filesystem::path& path) {
    tin::Data data;
    for (const auto& [id, frequency, deposite] : overlayPresets) {
        data.emplace(id, std::to_string(frequency) + '|' + std::to_string(deposite));
    }
    tin::write(path / "overlay.tin", data);
}

OverlayPresets PropertiesSaver::loadOverlayPreset(const std::filesystem::path& path) {
    const tin::Data data = tin::read(path / "overlay.tin");
    OverlayPresets overlayPresets;
    for (const auto& [id, strEntry] : data) {
        TileCoord tcEntry = validator::toTileCoord(strEntry).value_or(TileCoord(0, 0));
        overlayPresets.emplace_back(id, tcEntry.x, tcEntry.y);
    }
    return overlayPresets;
}

void PropertiesSaver::save(const WorldProperties& properties, const std::filesystem::path& path) {
    tin::Data data;
    data.emplace("map_size", std::to_string(properties.mapSize.x) + '|' + std::to_string(properties.mapSize.y));
    data.emplace("seed", std::to_string(properties.seed));
    tin::write(path / "world_properties.tin", data);
    saveOverlayPreset(properties.overlayPresets, path);
}

WorldProperties PropertiesSaver::load(const std::filesystem::path& path) {
    const tin::Data data = tin::read(path / "world_properties.tin");
    TileCoord mapSize = data.getTileCoord("map_size").value_or(TileCoord(0, 0));
    uint64_t seed = data.getUint64("seed").value_or(0U);
    OverlayPresets overlayPresets = loadOverlayPreset(path);
    return WorldProperties(mapSize, seed, overlayPresets);
}
