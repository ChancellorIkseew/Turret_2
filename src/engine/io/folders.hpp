#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace io::folders {
    inline const std::filesystem::path RES = "res";
    inline const std::filesystem::path SAVES = "saves";
    inline const std::filesystem::path LANG = RES / "lang";
    inline const std::filesystem::path TEXTURES = RES / "textures";

    using Contents = std::vector<std::string>;
    enum class ContentsType {
        folder,
        file
    };

    bool isPathValid(const std::filesystem::path& path) noexcept;
    bool folderExists(const std::filesystem::path& path);
    bool createOrCheckFolder(const std::filesystem::path& path);
    Contents getContents(const std::filesystem::path& path, ContentsType type);
    void deleteFolder(const std::filesystem::path& path);
}
