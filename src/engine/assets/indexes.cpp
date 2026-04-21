#include "indexes.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/io/parser/validator.hpp"

static const std::string floorError = "floor_error";

void Indexes::load() {
    std::filesystem::path path = io::folders::CONTENT / "content_indexes.tin";
    if (!io::folders::fileExists(path)) // Should not be caught. Crash the game. 
        throw std::runtime_error("Could not find content indexes file. Path: " + path.string());
    tin::Data data = tin::read(path);
    for (const auto& [name, id] : data) {
        if (name.starts_with("floor_"))
            floor.emplace(name, validator::to<uint8_t>(id).value_or(0U));
    }
}

const std::string& Indexes::getFloorByIndex(const uint8_t index) const {
    for (const auto& [type, id] : floor) {
        if (index == id)
            return type;
    }
    return floorError;
}
