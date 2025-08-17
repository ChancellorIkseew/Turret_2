#include "folders.hpp"
//
#include "engine/debug/logger.hpp"

namespace fs = std::filesystem;
static debug::Logger logger("io_folders");

bool io::folders::isPathValid(const fs::path& path) noexcept {
    return !path.empty() && path.string().find_first_of(" \t\n\r\f\v\"*?<>|") == std::string::npos;
}

bool io::folders::folderExists(const std::filesystem::path& path) {
    bool exists = isPathValid(path) && fs::exists(path) && fs::is_directory(path);
    if (!exists)
        logger.error() << "Directory does not exist. Invalid path: " << path;
    return exists;
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

io::folders::Contents io::folders::getContents(const std::filesystem::path& path) {
    Contents contents;
    if (!folderExists(path))
        return contents;
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_directory())
            contents.push_back(entry.path().filename().string());
    }
    logger.info() << "Readen directory: " << path;
    return contents;
}

void io::folders::deleteFolder(const std::filesystem::path& path) {
    if (!folderExists(path))
        return;
    try {
        fs::remove_all(path);
        logger.info() << "Deleted directory: " << path;
    }
    catch (const fs::filesystem_error& exception) {
        logger.error() << "Failed to delete directory: " << path << "std::filesystem exception : " << exception.what();
    }  
}
