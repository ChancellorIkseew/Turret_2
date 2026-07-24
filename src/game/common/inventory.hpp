#pragma once
#include <array>
#include <cstddef>
#include "engine/assets/preset_id.hpp"

struct BlockPreset;

struct Inventory {
    static constexpr size_t MAX_ITEM_TYPES = 32;
    std::array<uint64_t, MAX_ITEM_TYPES> resources{};
public:
    constexpr uint64_t count(const ItemPresetID item) const noexcept {
        return resources[item.asUint()];
    }
    constexpr bool has(const ItemPresetID item, const uint64_t amount) const noexcept {
        return count(item) >= amount;
    }
    constexpr void waste(const ItemPresetID item, const uint64_t amount) noexcept {
        resources[item.asUint()] -= amount;
    }
    constexpr void add(const ItemPresetID item, const uint64_t amount) noexcept {
        resources[item.asUint()] += amount;
    }
    //
    int16_t getMaxBuildStep(const BlockPreset& preset) const;
    void consumeByBuild(const BlockPreset& preset, const int16_t currentProgress, const int16_t step);
    void refundByDemolish(const BlockPreset& preset, const int16_t currentProgress, const int16_t step);
};
