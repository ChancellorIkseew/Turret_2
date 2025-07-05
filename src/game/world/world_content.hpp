#pragma once
#include <string>
#include <unordered_map>

struct ElementRegistry {
    std::unordered_map<uint8_t, std::string> floorTypes;
    std::unordered_map<uint8_t, std::string> overlayTypes;
    std::unordered_map<uint16_t, std::string> blockTypes;
};

static void load(ElementRegistry& reg) {
    //reg.floorTypes.emplace(0, "void");
    reg.floorTypes.emplace(1, "snow");
    reg.floorTypes.emplace(2, "ice");
    reg.floorTypes.emplace(3, "water");
    reg.floorTypes.emplace(4, "soil");
    reg.floorTypes.emplace(5, "stone");
    reg.floorTypes.emplace(6, "magma");
    reg.floorTypes.emplace(7, "plate");

    reg.overlayTypes.emplace(1, "iron");
    reg.overlayTypes.emplace(1, "copper");
}
