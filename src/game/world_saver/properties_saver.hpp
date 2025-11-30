#pragma once
#include <filesystem>
#include "game/generation/generation.hpp"

namespace serializer {
    void save(const WorldProperties& properties, const std::filesystem::path& path);
    WorldProperties load(const std::filesystem::path& path);
}
