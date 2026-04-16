#include "io.hpp"
//
#include <fstream>
#include "engine/debug/logger.hpp"

static debug::Logger logger("io");

std::string io::readFile(const fs::path& path, const Log log) {
    std::error_code errCode;
    if (!fs::exists(path, errCode) || !fs::is_regular_file(path, errCode)) {
        logger.error() << "File issue: " << path << " " << errCode.message();
        return "";
    }
    //
    std::ifstream fin(path, std::ios::binary | std::ios::ate);
    if (!fin.is_open()) {
        logger.error() << "Failed to open to read file: " << path;
        return "";
    }
    //
    const auto size = fin.tellg();
    if (size <= 0) {
        logger.warning() << "File is empty: " << path;
        return "";
    }
    
    std::string buffer;
    buffer.resize(static_cast<size_t>(size));
    fin.seekg(0, std::ios::beg);
    fin.read(buffer.data(), size);
    if (log == Log::error_and_success)
        logger.info() << "Readen file: " << path;
    return buffer;
}

bool io::writeFile(const fs::path& path, const std::string_view text, const Log log) {
    std::ofstream fout(path, std::ios::binary);
    if (!fout.is_open()) {
        logger.error() << "Failed to open to write file : " << path;
        return false;
    }
    fout << text;
    logger.info() << "Writen file: " << path;
    return fout.good();
}
