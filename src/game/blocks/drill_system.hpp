#pragma once
#include "blocks.hpp"

using Item = uint8_t;
using ItemCount = int64_t;

struct DrillSoA {
    std::vector<TileCoord> tile;
    std::vector<ItemCount> itemsPerSecond;
    struct Inventory {
        std::vector<Item> item;
        std::vector<ItemCount> count;
        std::vector<ItemCount> maxCount;
    } inventory;
    size_t drillCount = 0;
};


#include "game/world/world_map.hpp"

void mine(DrillSoA& soa, const WorldMap& map) {
    const size_t drillCount = soa.drillCount;
    for (size_t i = 0; i < drillCount; ++i) {

        soa.inventory.item[i] = map.at(soa.tile[i]).floor; //expand



        if (soa.inventory.count[i] < soa.inventory.maxCount[i])
            soa.inventory.count[i] += soa.itemsPerSecond[i];

    }
}

void provide(DrillSoA& soa) {
    const size_t drillCount = soa.drillCount;
    for (size_t i = 0; i < drillCount; ++i) {
        if (soa.inventory.count[i] <= 0)
            continue;
        //expand
    }
}


struct ConveyorSoA{
    std::vector<TileCoord> tile;
    std::vector<ItemCount> itemsPerSecond;



    size_t conveyorCount = 0;
};


void provide(ConveyorSoA& soa) {
    const size_t conveyorCount = soa.conveyorCount;
    for (size_t i = 0; i < conveyorCount; ++i) {
        //if (soa.inventory.count[i] <= 0)
            //continue;
        //expand
    }
}
