#pragma once
#include "game/generation/generation.hpp"

namespace std::filesystem { class path; }

namespace serializer {
    void saveFloorPreset(const FloorPresets& floorPresets, const std::filesystem::path& path);
    FloorPresets loadFloorPreset(const std::filesystem::path& path);
    //
    void saveOverlayPreset(const OverlayPresets& overlayPresets, const std::filesystem::path& path);
    OverlayPresets loadOverlayPreset(const std::filesystem::path& path);
}
