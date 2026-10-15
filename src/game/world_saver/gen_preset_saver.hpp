#pragma once
#include <filesystem>
#include "game/generation/generation.hpp"

namespace serializer {
    void saveFloorPreset(const FloorGenPresets& floorPresets, const std::filesystem::path& path);
    FloorGenPresets loadFloorPreset(const std::filesystem::path& path);
    //
    void saveOverlayPreset(const OreGenPresets& overlayPresets, const std::filesystem::path& path);
    OreGenPresets loadOverlayPreset(const std::filesystem::path& path);
}
