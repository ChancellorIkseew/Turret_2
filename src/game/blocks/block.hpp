#pragma once
#include "engine/assets/preset_id.hpp"
#include "engine/render/texture.hpp"
#include "game/common/physics_base.hpp"

class BlockMap;
class Renderer;

struct ItemStack {
    uint8_t item = 0;
    uint8_t count = 0;
};

enum class BlockType {
    air,
    belt,
    wall,
    drill,
    factory,
    turret,
    core
};

enum BlockRot : uint8_t {
    down  = 0,
    right = 1,
    up    = 2,
    left  = 3,
    none  = 255
};

struct Block {
    Health health = 0;
    Texture texture = NULL_TEXTURE;
    //
    virtual ~Block() = default;
    virtual BlockType getType() const noexcept = 0;
    virtual void draw(const Renderer& renderer, TileCoord tile);
    virtual bool canAccept(uint8_t item, BlockRot srcRot) { return false; }
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

struct BeltBlock : Block {

    uint8_t ids[3]; // Item IDs
    float ys[3]; // progress 0.0-1.0
    float xs[3];
    int len; // itemCount
    float minitem = 0.f; //last item progress
    int mid = 0; //current central item
    int lastInserted = 0;

    static constexpr float ITEM_SPACE = 4.f;
    static constexpr int CAPACITY = 3;

    Block* next = nullptr; // any block
    BeltBlock* nextBelt = nullptr;

    BlockRot rotation = left;
    bool aligned = true;
    //
    BeltBlock(BlockRot rotation) : rotation(rotation) {}
    virtual ~BeltBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::belt; }
    //
    virtual void draw(const Renderer& renderer, TileCoord tile);
    virtual bool canAccept(uint8_t item, BlockRot srcRot) final;
    void update(TileCoord tile, const BlockMap& map);
private:
    static BeltBlock* findNext(TileCoord tile, const BlockMap& map) noexcept;
    void moveItems();
    bool pass(uint8_t item, BlockRot srcRot);
};

struct FactoryBlock : Block {
    virtual ~FactoryBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::factory; }
};

struct TurretBlock : Block {
    TurretPresetID turretPreset;
    //
    TurretBlock(TurretPresetID turretPreset) : turretPreset(turretPreset) {}
    virtual ~TurretBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::turret; }
};

struct WallBlock : Block {
    virtual ~WallBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::wall; }
};
