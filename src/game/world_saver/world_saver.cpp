#include "world_saver.hpp"
//
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
#include "game/world/world.hpp"
#include "map_saver.hpp"
#include "serializer_error.hpp"

static debug::Logger logger("serializer");

void serializer::saveWorld(const World& world, const std::string& folder) {
    try {
        if (!io::folders::isPathValid(folder))
            throw SerializerError("Invalid folder name: " + folder);
        if (!io::folders::createOrCheckFolder(io::folders::SAVES))
            throw SerializerError("Could not find or create folder" + io::folders::SAVES.string());
        const std::filesystem::path path = io::folders::SAVES / folder;
        if (!io::folders::createOrCheckFolder(path))
            throw SerializerError("Could not find or create folder " + path.string());
        //
        serializer::saveMap(world.getMap(), path);
        logger.info() << "World successfully saved.";
    }
    catch (SerializerError& exception) {
        logger.error() << "Failed to save world. " << exception.what();
    }
}

std::unique_ptr<World> serializer::loadWorld(const std::string& folder) {
    try {
        if (!io::folders::isPathValid(folder))
            throw SerializerError("Invalid folder name: " + folder);
        if (!io::folders::folderExists(io::folders::SAVES))
            throw SerializerError("Could not find folder" + io::folders::SAVES.string());
        const std::filesystem::path path = io::folders::SAVES / folder;
        if (!io::folders::folderExists(path))
            throw SerializerError("Could not find folder " + path.string());
        //
        auto map = serializer::loadMap(path);
        logger.info() << "World successfully load.";
        return std::make_unique<World>(map);
    }
    catch (SerializerError& exception) {
        logger.error() << "Failed to load world. " << exception.what();
        return nullptr;
    }
}
