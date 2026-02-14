#include "jtin_parser.hpp"
//
#include <fstream>
#include "engine/debug/logger.hpp"

static debug::Logger logger("jtin_parser");

void jtin::write(std::ostream& text, const Data& data) {
    for (const tin::Data& block : data) {
        text << "{\n";
        tin::write(text, block);
        text << "}\n";
    }
}

jtin::Data jtin::read(std::istream& text) {
    std::vector<tin::Data> data;
    std::string line;
    while (std::getline(text, line)) {
        if (line != "{")
            continue;
        std::string blockContent;
        while (std::getline(text, line) && line != "}") {
            blockContent += line + "\n";
        }
        std::istringstream blockStream(blockContent);
        data.push_back(tin::read(blockStream));
    }
    return data;
}

void jtin::write(const fs::path& path, const jtin::Data& data, const Log log) {
    std::ofstream fout(path);
    if (!fout.is_open()) {
        logger.error() << "Could not open file to write. File: " << path;
        return;
    }
    std::stringstream fileText;
    write(fileText, data);
    fout << fileText.str();
    if (log == Log::error_and_success)
        logger.info() << "Writen file: " << path;
}

jtin::Data jtin::read(const fs::path& path, const Log log) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        logger.error() << "Could not open file to read. File: " << path;
        return Data();
    }
    const Data data = read(fin);
    if (log == Log::error_and_success)
        logger.info() << "Readen file: " << path;
    return data;
}
