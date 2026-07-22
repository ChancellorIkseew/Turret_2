#pragma once
#include <array>
#include "engine/assets/preset_id.hpp"

struct Inventory {
    static constexpr size_t MAX_ITEM_TYPES = 32;
    std::array<uint64_t, MAX_ITEM_TYPES> resources{};
public:
    constexpr bool has(const ItemPresetID item, const uint64_t amount) const noexcept {
        return resources[item.asUint()] >= amount;
    }
    constexpr void waste(const ItemPresetID item, const uint64_t amount) noexcept {
        resources[item.asUint()] -= amount;
    }
};
