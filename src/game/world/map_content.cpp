#include "map_content.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/io/parser/validator.hpp"

void content::load(ElementRegistry& reg) {
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
