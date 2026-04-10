#pragma once
#include "engine/render/texture.hpp"
#include "game/common/physics_base.hpp"

class Renderer;

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
    Health health = 0;
    Texture texture = NULL_TEXTURE;
    //
    virtual ~Block() = default;
    virtual BlockType getType() const noexcept = 0;
    virtual void draw(const Renderer& renderer, TileCoord tile);
};

struct CoreBlock : Block {
    virtual ~CoreBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::core; }
};

struct DrillBlock : Block {
    uint8_t itemPerTick;
    TickCount restReloadTime;
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

struct WallBlock : Block {
    virtual ~WallBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::wall; }
};
