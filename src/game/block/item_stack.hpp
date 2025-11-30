#pragma once
#include <cstdint>

using Item = uint16_t;
using ItemCount = uint16_t;

class ItemStack {
    Item item = 0U;
    ItemCount count = 0U;
public:
    ItemStack(const Item item, const ItemCount count) :
        item(item), count(count) { }
};

#include <vector>

using Inventory = std::vector<ItemStack>;
