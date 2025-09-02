#include "list_parser.hpp"
//
#include <fstream>
#include "engine/debug/logger.hpp"

namespace fs = std::filesystem;
static debug::Logger logger("list_parser");

void list::write(fs::path path, const list::Data& data) {
    std::ofstream fout(path);
    if (!fout.is_open()) {
        logger.error() << "Could not open file to write. File: " << path;
        return;
    }

    std::stringstream fileText;
    for (const auto& str : data) {
        fileText << str << '\n';
    }
    fout << fileText.str();
    logger.info() << "Writen file: " << path;
}

list::Data list::read(fs::path path) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        logger.error() << "Could not open file to read. File: " << path;
        return list::Data();
    }

    list::Data data;
    std::string line;
    while (std::getline(fin, line)) {
        if (!line.empty() && line[0] != '#')
            data.push_back(line);
    }
    logger.info() << "Readen file: " << path;
    return data;
}
