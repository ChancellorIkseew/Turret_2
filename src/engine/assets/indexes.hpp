#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

class Indexes {
    std::unordered_map<std::string, uint8_t> floor;
    std::unordered_map<std::string, uint8_t> overlay;
    std::unordered_map<std::string, uint16_t> blocks;
public:
    const auto& getFloor() const { return floor; }
    const auto& getOverlay() const { return overlay; }
    const auto& getBlocks() const { return blocks; }
    const std::string& getFloorByIndex(const uint8_t index) const;
    const std::string& getOverlayByIndex(const uint8_t index) const;
    const std::string& getBlockByIndex(const uint16_t index) const;
    void load();
};
