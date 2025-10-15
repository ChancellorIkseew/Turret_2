#pragma once
#include "game/generation/generation.hpp"

namespace std::filesystem { class path; }
struct ContentIndexes;

namespace serializer {
    void saveContentIndexes(const ContentIndexes& indexes, const std::filesystem::path& path);
    ContentIndexes  loadContentIndexes(const std::filesystem::path& path);
    //
    void save(const WorldProperties& properties, const std::filesystem::path& path);
    WorldProperties load(const std::filesystem::path& path);
}
