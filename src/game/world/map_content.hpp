#pragma once
#include <string>
#include <unordered_map>

struct ElementRegistry {
    std::unordered_map<std::string, uint8_t> floorTypes;
    std::unordered_map<std::string, uint8_t> overlayTypes;
    std::unordered_map<std::string, uint16_t> blockTypes;
};

namespace content {
    void load(ElementRegistry& reg);
}
