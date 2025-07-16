#pragma once
#include <string>
#include <unordered_map>

struct ElementRegistry {
    std::unordered_map<uint8_t, std::string> floorTypes;
    std::unordered_map<uint8_t, std::string> overlayTypes;
    std::unordered_map<uint16_t, std::string> blockTypes;
};

namespace content {
    void load(ElementRegistry& reg);
}
