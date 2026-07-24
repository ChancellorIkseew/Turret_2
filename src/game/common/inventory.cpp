#include "inventory.hpp"
//
#include "engine/assets/preset_defs.hpp"

int16_t Inventory::getMaxBuildStep(const BlockPreset& preset) const {
    const int32_t totalTime = std::max<int16_t>(1, preset.buildTime);
    int16_t minStep = std::numeric_limits<int16_t>::max();
    for (const auto& ing : preset.recipe) {
        if (ing.itemID.asUint() == 0)
            break;
        if (ing.amount <= 0)
            continue;
        const int32_t currentAmount = static_cast<int32_t>(count(ing.itemID));
        const int32_t possibleStep = (currentAmount * totalTime) / ing.amount;
        minStep = std::min<int16_t>(minStep, static_cast<int16_t>(possibleStep));
    }
    return (minStep == std::numeric_limits<int16_t>::max()) ? preset.buildTime : minStep;
}

void Inventory::consumeByBuild(const BlockPreset& preset, const int16_t currentProgress, const int16_t step) {
    if (step <= 0)
        return;
    const int32_t totalTime = std::max<int16_t>(1, preset.buildTime);
    const int32_t currentProgress32 = currentProgress;
    const int32_t step32 = step;
    for (const auto& ing : preset.recipe) {
        if (ing.itemID.asUint() == 0)
            break;
        const int32_t amount = ing.amount;
        const int16_t totalTargetWaste = static_cast<int16_t>((amount * (currentProgress32 + step32)) / totalTime);
        const int16_t totalAlreadyWasted = static_cast<int16_t>((amount * currentProgress32) / totalTime);
        const int16_t resourceToWaste = totalTargetWaste - totalAlreadyWasted;
        waste(ing.itemID, resourceToWaste);
    }
}

void Inventory::refundByDemolish(const BlockPreset& preset, const int16_t currentProgress, const int16_t step) {
    if (step <= 0)
        return;
    const int32_t totalTime = std::max<int16_t>(1, preset.buildTime);
    const int32_t currentProgress32 = currentProgress;
    const int32_t step32 = step;
    for (const auto& ing : preset.recipe) {
        if (ing.itemID.asUint() == 0)
            break;
        const int32_t amount = ing.amount;
        const int16_t wastedBefore = static_cast<int16_t>((amount * currentProgress32) / totalTime);
        const int16_t wastedAfter = static_cast<int16_t>((amount * (currentProgress32 - step32)) / totalTime);
        const int16_t resourceToReturn = wastedBefore - wastedAfter;
        add(ing.itemID, resourceToReturn);
    }
}
