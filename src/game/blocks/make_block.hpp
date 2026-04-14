#pragma once
#include <memory>
#include "block.hpp"
#include "engine/assets/preset_defs.hpp"
#include "engine/debug/logger.hpp"

inline std::unique_ptr<Block> makeBlock(BlockPreset preset, BlockRot rotation) {
    static debug::Logger bmLogger("bloc_make");
    std::unique_ptr<Block> block;
    switch (preset.archetype) {
    case BlockType::belt:
        block = std::make_unique<BeltBlock>(rotation);
        break;
    case BlockType::core:
        block = std::make_unique<CoreBlock>();
        break;
    case BlockType::drill:
        block = std::make_unique<DrillBlock>();
        break;
    case BlockType::factory:
        block = std::make_unique<FactoryBlock>();
        break;
    case BlockType::router:
        block = std::make_unique<RouterBlock>();
        break;
    case BlockType::turret:
        block = std::make_unique<TurretBlock>(preset.turret);
        break;
    default:
        bmLogger.error() << "Could not handle block type";
    case BlockType::wall:
        block = std::make_unique<WallBlock>();
        break;
    }

    block->health = preset.maxHealth;
    block->texture = preset.visual.texture;
    return block;
}
