#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace io::folders {
    namespace fs = std::filesystem;
    extern const fs::path RES;
    extern const fs::path SAVES;
    extern const fs::path SCREENSHOTS;
    extern const fs::path LANG;
    extern const fs::path CURSORS;
    extern const fs::path IMAGES;
    extern const fs::path FONTS;
    extern const fs::path CONTENT;
    extern const fs::path GENERATION;
    extern const fs::path GENERATION_DEFAULT;

    using Contents = std::vector<std::string>;
    enum class ContentsType : uint8_t { folder, file };

    bool isPathValid(const fs::path& path) noexcept;
    bool folderExists(const fs::path& path);
    bool fileExists(const fs::path& path);
    ///@brief Returns true if folder already exists or folder was
    /// successfully created by this function.
    bool createOrCheckFolder(const fs::path& path);
    Contents getContents(const fs::path& path, const ContentsType type);
    void deleteFolder(const fs::path& path);
}
