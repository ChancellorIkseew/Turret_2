#include "tin_parser.hpp"
//
#include <fstream>
#include "engine/debug/logger.hpp"

namespace fs = std::filesystem;
static debug::Logger logger("tin_parser");

void tin::write(fs::path path, const tin::Data& data) {
    std::ofstream fout(path);
    if (!fout.is_open()) {
        logger.error() << "Could not open file to write. File: " << path;
        return;
    }

    std::stringstream fileText;
    for (const auto& [key, value] : data) {
        fileText << key << '=' << value << '\n';
    }
    fout << fileText.str();
    logger.info() << "Writen file: " << path;
}

tin::Data tin::read(fs::path path) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        logger.error() << "Could not open file to read. File: " << path;
        return tin::Data();
    }

    tin::Data data;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty() || line[0] == '#')
            continue;
        const size_t eqv = line.find_first_of('=');
        if (eqv == line.npos)
            continue;
        std::string key = line.substr(0, eqv);
        std::string value = line.substr(eqv + 1);
        data.emplace(std::move(key), std::move(value));
    }
    logger.info() << "Readen file: " << path;
    return data;
}
