#include "world_saver.hpp"
//
#include "engine/io/folders.hpp"
#include "game/world/world.hpp"
#include "map_saver.hpp"

void serializer::saveWorld(const World& world, const std::string& folder) {
    if (!io::folders::createOrCheckFolder(io::folders::SAVES))
        return;
    const std::filesystem::path path = io::folders::SAVES / folder;
    if (!io::folders::createOrCheckFolder(path))
        return;
    serializer::saveMap(world.getMap(), path);
}

std::unique_ptr<World> serializer::loadWorld(const std::string& folder) {
    if (!io::folders::folderExists(io::folders::SAVES))
        return nullptr; // TODO: replace nullptrs with exceptions
    const std::filesystem::path path = io::folders::SAVES / folder;
    if (!io::folders::folderExists(path))
        return nullptr;
    auto map = serializer::loadMap(path);
    return std::make_unique<World>(map);
}
