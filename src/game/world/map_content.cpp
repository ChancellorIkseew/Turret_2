#include "map_content.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/io/parser/validator.hpp"

static const std::string floorError = "floor_error";
static const std::string overlayError = "overlay_error";
static const std::string blockError = "block_error";

void content::load(ContentIndexes& reg) {
    tin::Data data = tin::read(io::folders::RES / "map_content.tin");
    for (const auto& [name, id] : data) {
        if (name.find("floor_") != std::string::npos)
            reg.floorTypes.emplace(name, validator::toUint8(id).value_or(0U));
        else if (name.find("overlay_") != std::string::npos)
            reg.overlayTypes.emplace(name, validator::toUint8(id).value_or(0U));
        else if (name.find("block_") != std::string::npos)
            reg.blockTypes.emplace(name, validator::toUint16(id).value_or(0U));
    }
}

const std::string& ContentIndexes::getFloorByIndex(const uint8_t index) const {
    for (const auto& [type, id] : floorTypes) {
        if (index == id)
            return type;
    }
    return floorError;
}
const std::string& ContentIndexes::getOverlayByIndex(const uint8_t index) const {
    for (const auto& [type, id] : overlayTypes) {
        if (index == id)
            return type;
    }
    return overlayError;
}
const std::string& ContentIndexes::getBlockByIndex(const uint16_t index) const {
    for (const auto& [type, id] : blockTypes) {
        if (index == id)
            return type;
    }
    return blockError;
}
