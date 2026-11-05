#include "world_saver.hpp"
//
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
#include "game/world/world.hpp"
#include "blocks_saver.hpp"
#include "map_saver.hpp"
#include "mobs_saver.hpp"
#include "schematic_saver.hpp"
#include "shells_saver.hpp"
#include "teams_saver.hpp"
#include "time_count_saver.hpp"
#include "turrets_saver.hpp"
#include "world_config_saver.hpp"

static debug::Logger logger("serializer");

void serializer::saveWorld(const World& world, const std::string& folder) {
    try {
        if (!io::folders::isPathValid(folder))
            throw std::runtime_error("Invalid folder name: " + folder);
        if (!io::folders::createOrCheckFolder(io::folders::SAVES))
            throw std::runtime_error("Could not find or create folder" + io::folders::SAVES.string());
        const std::filesystem::path path = io::folders::SAVES / folder;
        if (!io::folders::createOrCheckFolder(path))
            throw std::runtime_error("Could not find or create folder " + path.string());
        //
        saveConfig(world.getConfig(), path);
        saveTimeCount(world.getTimeCount(), path);
        serializer::saveMap(world.getMap(), path);
        saveBlocks(world.getBlocks(), path);
        saveMobs(world.getMobs().getSoa(), path);
        saveShells(world.getShells().getSoa(), path);
        saveTurrets(world.getBlocks().getMeta().getTurrets().getSoa(), path);
        saveSchematic(world.getSchematic(), path);
        saveTeams(world.getTeams(), path);
        logger.info("World successfully saved.");
    }
    catch (const std::exception& exception) {
        logger.error("Failed to save world. {}", exception.what());
    }
}

std::unique_ptr<World> serializer::loadWorld(const std::string& folder, const Presets& presets) {
    try {
        if (!io::folders::isPathValid(folder))
            throw std::runtime_error("Invalid folder name: " + folder);
        if (!io::folders::folderExists(io::folders::SAVES))
            throw std::runtime_error("Could not find folder" + io::folders::SAVES.string());
        const std::filesystem::path path = io::folders::SAVES / folder;
        if (!io::folders::folderExists(path))
            throw std::runtime_error("Could not find folder " + path.string());
        //
        WorldMap map = serializer::loadMap(path);
        auto world = std::make_unique<World>(map, loadConfig(path), loadTimeCount(path));
        // CRITICAL: load blocks BEFORE turrets.
        // place() during block loading creates temporary turret SoA entries
        // → loadTurrets() overwrites the entire SoA with saved data.
        loadBlocks(world->getBlocks(), presets, path);
        world->getMobs().getSoa() = loadMobs(path);
        world->getShells().getSoa() = loadShells(path);
        world->getBlocks().getMeta().getTurrets().getSoa() = loadTurrets(path);
        loadSchematic(world->getSchematic(), path);
        loadTeams(world->getTeams(), path);
        logger.info("World successfully loaded.");
        return world;
    }
    catch (const std::exception& exception) {
        logger.error("Failed to load world. {}", exception.what());
        return nullptr;
    }
}
