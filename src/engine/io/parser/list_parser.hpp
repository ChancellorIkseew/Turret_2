#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace list {
    using Data = std::vector<std::string>;

    ///@brief Data vector can contain commentaries.
    /// Commentaries must begin with "#" symbol.
    void write(std::filesystem::path path, const Data& data);
    Data read(std::filesystem::path path);
}
