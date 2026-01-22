#include "gen_preset_saver.hpp"
//
#include "engine/io/parser/tin_parser.hpp"

void serializer::saveFloorPreset(const FloorPresets& floorPresets, const std::filesystem::path& path) {
    tin::Data data;
    for (const auto& [name, height] : floorPresets) {
        data.emplace(name, std::to_string(height));
    }
    tin::write(path / "floor.tin", data);
}

FloorPresets serializer::loadFloorPreset(const std::filesystem::path& path) {
    tin::Data data = tin::read(path / "floor.tin");
    FloorPresets floorPresets;
    for (const auto& [name, height] : data) {
        floorPresets.emplace_back(name, validator::to<float>(height).value_or(1.0f));
    }
    return floorPresets;
}

void serializer::saveOverlayPreset(const OverlayPresets& overlayPresets, const std::filesystem::path& path) {
    tin::Data data;
    for (const auto& [name, frequency, deposite] : overlayPresets) {
        data.emplace(name, std::to_string(frequency) + '|' + std::to_string(deposite));
    }
    tin::write(path / "overlay.tin", data);
}

OverlayPresets serializer::loadOverlayPreset(const std::filesystem::path& path) {
    const tin::Data data = tin::read(path / "overlay.tin");
    OverlayPresets overlayPresets;
    for (const auto& [name, strEntry] : data) {
        TileCoord tcEntry = validator::to<TileCoord>(strEntry).value_or(TileCoord(0, 0));
        overlayPresets.emplace_back(name, tcEntry.x, tcEntry.y);
    }
    return overlayPresets;
}
