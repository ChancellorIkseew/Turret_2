#pragma once
#include "engine/assets/preset_id.hpp"
#include "engine/coords/tile_coord.hpp"
#include "engine/render/texture_rect.hpp"
#include "game/common/physics_base.hpp"
#define t1_derived

class BlocksDrawer;
class BlockMap;
class Presets;
class Renderer;
class WorldMap;

struct ItemStack {
    ItemPresetID item = ItemPresetID(0);
    uint8_t count = 0;
};

enum class BPAction : uint8_t { build, demolish };

// order of types does mater
enum class BlockType {
    air,
    in_progress,
    wall,
    link,
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
    int size = 1;
    TextureRect textureRect = NULL_TEXTURE_RECT;
    //
    virtual ~Block() = default;
    virtual BlockType getType() const noexcept = 0;
    virtual BlockRot getRotation() const noexcept { return BlockRot::none; }
    virtual void draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile);
    virtual bool canAccept(ItemPresetID item, BlockRot srcRot) { return false; }
    virtual void accept(ItemPresetID item, BlockRot srcRot) {}
};

struct LinkBlock : Block {
    TileCoord masterTile;
    Block* master = nullptr;
    //
    LinkBlock(TileCoord masterTile, Block* master) : masterTile(masterTile), master(master) {}
    t1_derived BlockType getType() const noexcept final { return master->getType(); }
    t1_derived BlockRot getRotation() const noexcept final { return master->getRotation(); }
    //
    t1_derived void draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile) final;
    t1_derived bool canAccept(ItemPresetID item, BlockRot srcRot) final { return master->canAccept(item, srcRot); };
    t1_derived void accept(ItemPresetID item, BlockRot srcRot) final { master->accept(item, srcRot); };
};

struct InProgress : Block {
    BPAction action = BPAction::build;
    BlockRot rotation = none;
    int16_t progress = 0;
    //
    t1_derived BlockType getType() const noexcept final { return BlockType::in_progress; }
    t1_derived void draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile) final;
    void drawProgress(Renderer& renderer, const Presets& presets, TileCoord tile) const;
    //
    void increeseProgress(const int16_t step) noexcept { progress += (action == BPAction::build) ? step : -step; }
    bool isProgressFull(const int16_t buildTime) const noexcept {
        return (action == BPAction::build) ? (progress >= buildTime) : (progress <= 0);
    }
};

struct CoreBlock : Block {
    t1_derived BlockType getType() const noexcept final { return BlockType::core; }
};

struct DrillBlock : Block {
    uint8_t itemPerTick;
    TickCount restReloadTime;
    t1_derived BlockType getType() const noexcept final { return BlockType::drill; }
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
    t1_derived BlockType getType() const noexcept final { return BlockType::belt; }
    t1_derived BlockRot getRotation() const noexcept final { return rotation; }
    //
    t1_derived void draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile) final;
    t1_derived bool canAccept(ItemPresetID item, BlockRot srcRot) final;
    t1_derived void accept(ItemPresetID item, BlockRot srcRot) final;
    void update(TileCoord tile, const BlockMap& map);
private:
    void moveItems();
    bool pass(ItemPresetID item, BlockRot srcRot);
};

struct FactoryBlock : Block {
    t1_derived BlockType getType() const noexcept final { return BlockType::factory; }
};

struct TurretBlock : Block {
    TurretPresetID turretPreset;
    BlockRot defaultRotation;
    //
    TurretBlock(TurretPresetID turretPreset, BlockRot rotation) :
        turretPreset(turretPreset), defaultRotation(rotation) {}
    t1_derived BlockType getType() const noexcept final { return BlockType::turret; }
};

struct IntersectionBlock : Block {
    struct RotatedItem {
        ItemPresetID item = ItemPresetID(0);
        BlockRot rotation = BlockRot::none;
    };
    RotatedItem vertical, horizontal;
    //
    t1_derived BlockType getType() const noexcept final { return BlockType::intersection; }
    //
    t1_derived bool canAccept(ItemPresetID item, BlockRot srcRot) final;
    t1_derived void accept(ItemPresetID item, BlockRot srcRot) final;
    void provide(TileCoord tile, const BlockMap& map);
};

struct RouterBlock : Block {
    ItemStack inventory;
    //
    t1_derived BlockType getType() const noexcept final { return BlockType::router; }
    //
    t1_derived bool canAccept(ItemPresetID item, BlockRot srcRot) final { return inventory.count == 0; }
    t1_derived void accept(ItemPresetID item, BlockRot srcRot) final { inventory.item = item; inventory.count = 1; }
    void provide(TileCoord tile, const BlockMap& map);
};

struct WallBlock : Block {
    /*virt*/ BlockType getType() const noexcept final { return BlockType::wall; }
};

#undef t1_derived
