#pragma once
#include <memory>
#include "block_types.hpp"
#include "engine/assets/preset_defs.hpp"
#include "engine/debug/logger.hpp"

class Presets;

inline std::unique_ptr<Block> makeBlock(const Presets& presets, BlockPresetID presetID, BlockRot rotation) {
    static debug::Logger bmLogger("bloc_make");
    const BlockPreset preset = presets.getBlock(presetID);
    std::unique_ptr<Block> block;
    switch (preset.archetype) {
    case BlockType::belt:
        block = std::make_unique<BeltBlock>(rotation);
        break;
    case BlockType::core:
        block = std::make_unique<CoreBlock>();
        break;
    case BlockType::drill:
        block = std::make_unique<DrillBlock>(preset.productionTime);
        break;
    case BlockType::impact_drill:
        block = std::make_unique<ImpactDrillBlock>(preset.productionTime);
        break;
    case BlockType::factory:
        block = std::make_unique<FactoryBlock>(preset);
        break;
    case BlockType::junction:
        block = std::make_unique<JunctionBlock>();
        break;
    case BlockType::router:
        block = std::make_unique<RouterBlock>();
        break;
    case BlockType::turret:
        block = std::make_unique<TurretBlock>(presets.getTurret(preset.turret).ammo, preset.turret);
        break;
    default:
        bmLogger.error("Could not handle block type");
    case BlockType::wall:
        block = std::make_unique<WallBlock>();
        break;
    }

    block->presetID = presetID;
    block->size = preset.size;
    block->health = preset.maxHealth;
    block->textureRect = preset.visual.textureRect;
    return block;
}
