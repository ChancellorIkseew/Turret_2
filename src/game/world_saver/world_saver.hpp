#pragma once
#include <memory>
#include <string>

class World;

namespace serializer {
    void saveWorld(const World& world, const std::string& folder);
    std::unique_ptr<World> loadWorld(const std::string& folder);
}
