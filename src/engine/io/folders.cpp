#include "folders.hpp"
//
#include "engine/debug/logger.hpp"

namespace fs = std::filesystem;
static debug::Logger logger("io_folders");

bool io::folders::isPathValid(const fs::path& path) {
    return path.string().find_first_of(" \t\n\r\f\v\"*?<>|") == std::string::npos && !path.empty();
}

bool io::folders::folderExists(const std::filesystem::path& path) {
    if (!isPathValid(path)) {
        logger.error() << "Directory does not exist. Invalid symbols in path. Path: " << path;
        return false;
    }
    return fs::is_directory(path);
}

bool io::folders::createOrCheckFolder(const std::filesystem::path& path) {
    if (!isPathValid(path)) {
        logger.error() << "Failed to create directory. Invalid symbols in path. Path: " << path;
        return false;
    }

    try {
        // If a file with this path exists, but this file is not a directory,
        // "fs::create_directory" will throw an exception.
        if (fs::create_directory(path))
            logger.info() << "Created directory: " << path;
        else
            logger.info() << "Directory already exists: " << path;
        return true;
    }
    catch (const fs::filesystem_error& exception) {
        logger.error() << "Failed to create directory: std::filesystem exception: " << exception.what();
        return false;
    }
}
