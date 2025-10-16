#pragma once
#include "game/generation/generation.hpp"

namespace std::filesystem { class path; }

namespace serializer {
    void save(const WorldProperties& properties, const std::filesystem::path& path);
    WorldProperties load(const std::filesystem::path& path);
}
