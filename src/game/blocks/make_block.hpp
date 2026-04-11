#pragma once
#include <memory>
#include "block.hpp"
#include "engine/assets/preset_defs.hpp"

inline std::unique_ptr<Block> makeBlock(BlockPreset preset) {
    std::unique_ptr<Block> block;
    switch (preset.archetype) {
    case BlockType::belt:
        block = std::make_unique<BeltBlock>();
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
    case BlockType::turret:
        block = std::make_unique<TurretBlock>(preset.turret);
        break;
    case BlockType::wall:
        block = std::make_unique<WallBlock>();
        break;
    }

    block->health = preset.maxHealth;
    block->texture = preset.visual.texture;
    return block;
}
