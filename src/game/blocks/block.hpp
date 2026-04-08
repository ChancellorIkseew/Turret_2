#pragma once
#include "game/physics/physics_base.hpp"

struct ItemStack {
   
};

enum class BlockType {
    air,
    wall,
    drill,
    factory,
    turret,
    core
};

struct Block {
    virtual ~Block() = default;
    virtual BlockType getType() const noexcept = 0;
};

struct CoreBlock : Block {
    virtual ~CoreBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::core; }
};

struct DrillBlock : Block {
    uint8_t itemPerTick;
    TickCount reatReloadTime;
    virtual ~DrillBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::drill; }
public: //
    void mine(TickCount deltaTick) {

    }
};

struct FactoryBlock : Block {
    virtual ~FactoryBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::factory; }
};

struct TurretBlock : Block {
    virtual ~TurretBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::turret; }
};
