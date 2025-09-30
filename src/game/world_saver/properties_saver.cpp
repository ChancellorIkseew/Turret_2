#include "properties_saver.hpp"
//
#include "engine/debug/logger.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "game/generation/generation.hpp"

static debug::Logger logger("properties_saver");

static void saveOverlayPreset(const WorldProperties& properties, const std::filesystem::path& path) {
    tin::Data data;
    for (const auto/*not ref*/ [id, frequency, deposite] : properties.overlayPresets) {
        data.emplace(std::to_string(id), std::to_string(frequency) + '|' + std::to_string(deposite));
    }
    tin::write(path / "overlay.tin", data);
}

static std::vector<OverlayPreset> loadOverlayPreset(const std::filesystem::path& path) {
    const tin::Data data = tin::read(path / "overlay.tin");
    std::vector<OverlayPreset> overlayPresets;
    for (const auto& [strID, strEntry] : data) {
        TileCoord tcEntry = validator::toTileCoord(strEntry).value_or(TileCoord(0, 0));
        uint8_t id = validator::toUint8(strID).value_or(0U);
        overlayPresets.emplace_back(id, tcEntry.x, tcEntry.y);
    }
    return overlayPresets;
}

void PropertiesSaver::save(const WorldProperties& properties, const std::filesystem::path& path) {
    tin::Data data;
    data.emplace("map_size", std::to_string(properties.mapSize.x) + '|' + std::to_string(properties.mapSize.y));
    data.emplace("seed", std::to_string(properties.seed));
    tin::write(path / "world_properties.tin", data);
    saveOverlayPreset(properties, path);
}

WorldProperties PropertiesSaver::load(const std::filesystem::path& path) {
    const tin::Data data = tin::read(path / "world_properties.tin");
    TileCoord mapSize = data.getTileCoord("map_size").value_or(TileCoord(0, 0));
    uint64_t seed = data.getUint64("seed").value_or(0U);
    auto preset = loadOverlayPreset(path);
    return WorldProperties(mapSize, seed, preset);
}
