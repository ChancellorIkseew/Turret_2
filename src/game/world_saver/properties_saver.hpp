#pragma once
#include <filesystem>
#include "game/generation/generation.hpp"

struct ContentIndexes;

namespace serializer {
    void saveContentIndexes(const ContentIndexes& indexes, const std::filesystem::path& path);
    ContentIndexes  loadContentIndexes(const std::filesystem::path& path);
    //
    void saveOverlayPreset(const OverlayPresets& overlayPresets, const std::filesystem::path& path);
    OverlayPresets loadOverlayPreset(const std::filesystem::path& path);
    //
    void save(const WorldProperties& properties, const std::filesystem::path& path);
    WorldProperties load(const std::filesystem::path& path);
}
