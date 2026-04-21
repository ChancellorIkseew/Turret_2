#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

class Indexes {
    std::unordered_map<std::string, uint8_t> floor;
public:
    const auto& getFloor() const { return floor; }
    const std::string& getFloorByIndex(const uint8_t index) const;
    void load();
};
