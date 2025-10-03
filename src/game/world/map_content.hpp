#pragma once
#include <string>
#include <unordered_map>

struct ContentIndexes {
    std::unordered_map<std::string, uint8_t> floorTypes;
    std::unordered_map<std::string, uint8_t> overlayTypes;
    std::unordered_map<std::string, uint16_t> blockTypes;
    //
    const std::string& getFloorByIndex(const uint8_t index) const;
    const std::string& getOverlayByIndex(const uint8_t index) const;
    const std::string& getBlockByIndex(const uint16_t index) const;
};

namespace content {
    void load(ContentIndexes& reg);
}
