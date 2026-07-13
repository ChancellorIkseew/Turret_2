#pragma once
#include "engine/assets/preset_id.hpp"
#include "engine/coords/tile_coord.hpp"
#include "engine/render/texture_rect.hpp"
#include "game/common/physics_base.hpp"

class BlocksDrawer;
class BlockMap;
class Presets;
class Renderer;
class WorldMap;

struct ItemStack {
    ItemPresetID item = ItemPresetID(0);
    uint8_t count = 0;
};

// order of types does mater
enum class BlockType {
    air,
    in_progress,
    wall,
    belt,
    bridge,
    drill,
    factory,
    intersection,
    router,
    turret,
    core
};

enum BlockRot : int8_t {
    up    = 0,
    right = 1,
    down  = 2,
    left  = 3,
    none  = -1
};

static constexpr TileCoord DIR_VECS[] = {
    { 0, -1 }, // up
    { 1, 0 },  // right
    { 0, 1 },  // down
    { -1, 0 }  // left
};

struct Block {
    Health health = 0;
    BlockPresetID presetID = BlockPresetID(0);
    TextureRect textureRect = NULL_TEXTURE_RECT;
    //
    virtual ~Block() = default;
    virtual BlockType getType() const noexcept = 0;
    virtual BlockRot getRotation() const noexcept { return BlockRot::none; }
    virtual void draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile);
    virtual bool canAccept(ItemPresetID item, BlockRot srcRot) { return false; }
    virtual void accept(ItemPresetID item, BlockRot srcRot) {}
};

struct InProgress : Block {
    enum class BPAction : uint8_t { build, demolish };
    BPAction action = BPAction::build;
    BlockRot rotation = none;
    int8_t progress = 0;
    //
    virtual BlockType getType() const noexcept final { return BlockType::in_progress; }
    virtual void draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile);
    void drawInProgress(Renderer& renderer, const Presets& presets, TileCoord tile) const;
    bool progressFull() { return action == BPAction::build ? progress >= 100 : progress <= 0; }
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
    void throwItem(TileCoord tile, const BlockMap& map, const WorldMap& terrain, const Presets& presets);
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
    virtual BlockRot getRotation() const noexcept final { return rotation; }
    //
    virtual void draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile) final;
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
    BlockRot defaultRotation;
    //
    TurretBlock(TurretPresetID turretPreset, BlockRot rotation) :
        turretPreset(turretPreset), defaultRotation(rotation) {}
    virtual ~TurretBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::turret; }
};

struct IntersectionBlock : Block {
    struct RotatedItem {
        ItemPresetID item = ItemPresetID(0);
        BlockRot rotation = BlockRot::none;
    };
    RotatedItem vertical, horizontal;
    //
    virtual ~IntersectionBlock() final = default;
    virtual BlockType getType() const noexcept final { return BlockType::intersection; }
    //
    virtual bool canAccept(ItemPresetID item, BlockRot srcRot);
    virtual void accept(ItemPresetID item, BlockRot srcRot);
    void provide(TileCoord tile, const BlockMap& map);
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
