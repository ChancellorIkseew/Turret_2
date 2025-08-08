#pragma once
#include <filesystem>

namespace io::folders {
    inline const std::filesystem::path RES = "res";
    inline const std::filesystem::path SAVES = "saves";
    inline const std::filesystem::path TEXTURES = RES / "textures";

    bool isPathValid(const std::filesystem::path& path);
    bool folderExists(const std::filesystem::path& path);
    bool createOrCheckFolder(const std::filesystem::path& path);
}
