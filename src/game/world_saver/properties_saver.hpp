#pragma once
#include <filesystem>
#include "game/generation/generation.hpp"

class PropertiesSaver {
public:
    static void saveOverlayPreset(const OverlayPresets& overlayPresets, const std::filesystem::path& path);
    static OverlayPresets loadOverlayPreset(const std::filesystem::path& path);

    static void save(const WorldProperties& properties, const std::filesystem::path& path);
    static WorldProperties load(const std::filesystem::path& path);
};
