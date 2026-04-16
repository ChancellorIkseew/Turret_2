#pragma once
#include "engine/assets/preset_id.hpp"
#include "engine/render/texture.hpp"
#include "game/common/physics_base.hpp"

class BlocksDrawer;
class BlockMap;
class Renderer;
class WorldMap;

struct ItemStack {
    ItemPresetID item = ItemPresetID(0);
    uint8_t count = 0;
};

// order of types does mater
enum class BlockType {
    air,
    wall,
    belt,
    bridge,
    drill,
    factory,
    router,
    turret,
    core
};

enum BlockRot : int8_t {
    down  = 0,
    right = 1,
    up    = 2,
    left  = 3,
    none  = -1
};

struct Block {
    Health health = 0;
    Texture texture = NULL_TEXTURE;
    //
    virtual ~Block() = default;
    virtual BlockType getType() const noexcept = 0;
    virtual void draw(BlocksDrawer& blockDrawer, const Renderer& renderer, TileCoord tile);
    virtual bool canAccept(ItemPresetID item, BlockRot srcRot) { return false; }
    virtual void accept(ItemPresetID item, BlockRot srcRot) {}
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
    void throwItem(TileCoord tile, const BlockMap& map, const WorldMap& terrain);
};

struct BeltBlock : Block {
    ItemPresetID itemID[3]; // Item IDs
    float itemY[3]; //
    float itemX[3];
    float minItem = ITEM_SPACE; //last item progress

    int8_t len = 0; // itemCount
    int8_t mid = 0; //current central item
    int8_t lastInserted = 0;
    BlockRot rotation = left;
    bool aligned = true;

    static constexpr float ITEM_SPACE = 0.33f;
    static constexpr int8_t CAPACITY = 3;
    //
    BeltBlock(BlockRot rotation) : rotation(rotation) {}
    virtual ~BeltBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::belt; }
    //
    virtual void draw(BlocksDrawer& blockDrawer, const Renderer& renderer, TileCoord tile) final;
    virtual bool canAccept(ItemPresetID item, BlockRot srcRot) final;
    virtual void accept(ItemPresetID item, BlockRot srcRot) final;
    void update(TileCoord tile, const BlockMap& map);
private:
    void moveItems();
    bool pass(ItemPresetID item, BlockRot srcRot);
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

struct RouterBlock : Block {
    ItemStack inventory;
    //
    virtual ~RouterBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::router; }
    //
    virtual bool canAccept(ItemPresetID item, BlockRot srcRot) { return inventory.count == 0; }
    virtual void accept(ItemPresetID item, BlockRot srcRot) { inventory.item = item; inventory.count = 1; }
    void provide(TileCoord tile, const BlockMap& map);
};

struct WallBlock : Block {
    virtual ~WallBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::wall; }
};
