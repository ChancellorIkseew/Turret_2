#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace io::folders {
    inline const std::filesystem::path RES = "res";
    inline const std::filesystem::path SAVES = "saves";
    inline const std::filesystem::path SCREENSHOTS = "screenshots";
    inline const std::filesystem::path LANG = RES / "lang";
    inline const std::filesystem::path IMAGES = RES / "images";
    inline const std::filesystem::path FONTS = RES / "fonts";
    inline const std::filesystem::path GENERATION = RES / "generation";

    using Contents = std::vector<std::string>;
    enum class ContentsType : uint8_t {
        folder,
        file
    };

    bool isPathValid(const std::filesystem::path& path) noexcept;
    bool folderExists(const std::filesystem::path& path);
    ///@brief Returns true if folder already exists or folder was
    /// successfully created by this function.
    bool createOrCheckFolder(const std::filesystem::path& path);
    Contents getContents(const std::filesystem::path& path, const ContentsType type);
    void deleteFolder(const std::filesystem::path& path);
}
