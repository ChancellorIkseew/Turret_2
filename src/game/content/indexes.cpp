#include "indexes.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/io/parser/validator.hpp"

std::unordered_map<std::string, uint8_t> content::Indexes::floor;
std::unordered_map<std::string, uint8_t> content::Indexes::overlay;
std::unordered_map<std::string, uint16_t> content::Indexes::blocks;

static const std::string floorError = "floor_error";
static const std::string overlayError = "overlay_error";
static const std::string blockError = "block_error";

void content::Indexes::load() {
    tin::Data data = tin::read(io::folders::CONTENT / "content_indexes.tin");
    for (const auto& [name, id] : data) {
        if (name.find("floor_") != std::string::npos)
            floor.emplace(name, validator::toUint8(id).value_or(0U));
        else if (name.find("overlay_") != std::string::npos)
            overlay.emplace(name, validator::toUint8(id).value_or(0U));
        else if (name.find("block_") != std::string::npos)
            blocks.emplace(name, validator::toUint16(id).value_or(0U));
    }
}

const std::string& content::Indexes::getFloorByIndex(const uint8_t index) {
    for (const auto& [type, id] : floor) {
        if (index == id)
            return type;
    }
    return floorError;
}
const std::string& content::Indexes::getOverlayByIndex(const uint8_t index) {
    for (const auto& [type, id] : overlay) {
        if (index == id)
            return type;
    }
    return overlayError;
}
const std::string& content::Indexes::getBlockByIndex(const uint16_t index) {
    for (const auto& [type, id] : blocks) {
        if (index == id)
            return type;
    }
    return blockError;
}
