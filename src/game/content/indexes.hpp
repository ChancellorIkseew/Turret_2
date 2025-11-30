#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

namespace content {
    class Indexes {
        static std::unordered_map<std::string, uint8_t> floor;
        static std::unordered_map<std::string, uint8_t> overlay;
        static std::unordered_map<std::string, uint16_t> blocks;
    public:
        static const auto& getFloor() { return floor; }
        static const auto& getOverlay() { return overlay; }
        static const auto& getBlocks() { return blocks; }
        static const std::string& getFloorByIndex(const uint8_t index);
        static const std::string& getOverlayByIndex(const uint8_t index);
        static const std::string& getBlockByIndex(const uint16_t index);
        static void load();
    };
}
