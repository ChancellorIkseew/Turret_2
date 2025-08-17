#pragma once
#include <memory>
#include <string>

class World;

class WorldSaver {
public:
    static void save(const World& world, const std::string& folder);
    static std::unique_ptr<World> load(const std::string& folder);
};
