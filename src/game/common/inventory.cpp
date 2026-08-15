#include "inventory.hpp"
//
#include "engine/assets/preset_defs.hpp"

int16_t Inventory::getMaxBuildStep(const BlockPreset& preset) const {
    const int64_t totalTime = std::max<int16_t>(1, preset.buildTime);
    int64_t minStep = std::numeric_limits<int64_t>::max();
    for (const auto& ing : preset.recipe) {
        if (ing.itemID.asUint() == 0)
            break;
        if (ing.amount <= 0)
            continue;
        const int64_t amount = ing.amount;
        const int64_t currentAmount = count(ing.itemID);
        const int64_t possibleStep = (currentAmount * totalTime) / amount;
        minStep = std::min(minStep, possibleStep);
    }
    return static_cast<int16_t>(std::min<int64_t>(minStep, std::numeric_limits<int16_t>::max()));
}

void Inventory::consumeByBuild(const BlockPreset& preset, const int16_t currentProgress, const int16_t step) {
    if (step <= 0)
        return;
    const int64_t totalTime = std::max<int16_t>(1, preset.buildTime);
    const int64_t currentProgress64 = currentProgress;
    const int64_t step64 = step;
    for (const auto& ing : preset.recipe) {
        if (ing.itemID.asUint() == 0)
            break;
        const int64_t amount = ing.amount;
        const int16_t totalTargetWaste = static_cast<int16_t>((amount * (currentProgress64 + step64)) / totalTime);
        const int16_t totalAlreadyWasted = static_cast<int16_t>((amount * currentProgress64) / totalTime);
        const int16_t resourceToWaste = totalTargetWaste - totalAlreadyWasted;
        waste(ing.itemID, resourceToWaste);
    }
}

void Inventory::refundByDemolish(const BlockPreset& preset, const int16_t currentProgress, const int16_t step) {
    if (step <= 0)
        return;
    const int64_t totalTime = std::max<int16_t>(1, preset.buildTime);
    const int64_t currentProgress64 = currentProgress;
    const int64_t step64 = step;
    for (const auto& ing : preset.recipe) {
        if (ing.itemID.asUint() == 0)
            break;
        const int64_t amount = ing.amount;
        const int16_t wastedBefore = static_cast<int16_t>((amount * currentProgress64) / totalTime);
        const int16_t wastedAfter = static_cast<int16_t>((amount * (currentProgress64 - step64)) / totalTime);
        const int16_t resourceToReturn = wastedBefore - wastedAfter;
        add(ing.itemID, resourceToReturn);
    }
}
