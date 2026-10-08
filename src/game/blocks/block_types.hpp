#pragma once
#include "block.hpp"
#define t1_derived

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
    Team* team = nullptr;
    Health preveouseTickHealth = 0;
    //
    t1_derived BlockType getType() const noexcept final { return BlockType::core; }
    //
    t1_derived bool canAccept(ItemPresetID item, BlockRot srcRot) final { return true; };
    t1_derived void accept(ItemPresetID item, BlockRot srcRot) final;
    void syncTeam(Team* blockTeam) { team = blockTeam; }
};

struct DrillBlock : Block {
    ItemStack inventory;
    int8_t mineSpeed = 1;
    uint8_t step = 0;
    t1_derived BlockType getType() const noexcept final { return BlockType::drill; }
public:
    void mine(TileCoord tile, const WorldMap& terrain, const Presets& presets);
    void provide(TileCoord tile, const BlockMap& map);
};

struct ImpactDrillBlock : Block {
    ItemStack inventory;
    int8_t mineSpeed = 6;
    uint8_t step = 0;
    t1_derived BlockType getType() const noexcept final { return BlockType::impact_drill; }
public:
    void mine(TileCoord tile, const WorldMap& terrain, const Presets& presets);
    void provide(TileCoord tile, const BlockMap& map);
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
public:
    void produce(TileCoord tile, const WorldMap& terrain, const Presets& presets);
    void provide(TileCoord tile, const BlockMap& map);
};

struct TurretBlock : Block {
    TurretPresetID turretPreset;
    BlockRot defaultRotation;
    ItemStack ammo;
    //
    TurretBlock(TurretPresetID turretPreset, BlockRot rotation) :
        turretPreset(turretPreset), defaultRotation(rotation) {
    }
    t1_derived BlockType getType() const noexcept final { return BlockType::turret; }
    t1_derived bool canAccept(ItemPresetID item, BlockRot srcRot) final { return item == ItemPresetID(3) && ammo.count < 10; };
    t1_derived void accept(ItemPresetID item, BlockRot srcRot) final { ++ammo.count; };
    void useAmmo() { --ammo.count; }
};

struct JunctionBlock : Block {
    struct RotatedItem {
        ItemPresetID item = ItemPresetID(0);
        BlockRot rotation = BlockRot::none;
    };
    RotatedItem vertical, horizontal;
    //
    t1_derived BlockType getType() const noexcept final { return BlockType::junction; }
    //
    t1_derived bool canAccept(ItemPresetID item, BlockRot srcRot) final;
    t1_derived void accept(ItemPresetID item, BlockRot srcRot) final;
    void provide(TileCoord tile, const BlockMap& map);
};

struct RouterBlock : Block {
    ItemStack inventory;
    uint8_t step;
    //
    t1_derived BlockType getType() const noexcept final { return BlockType::router; }
    //
    t1_derived bool canAccept(ItemPresetID item, BlockRot srcRot) final { return inventory.count == 0; }
    t1_derived void accept(ItemPresetID item, BlockRot srcRot) final { inventory.item = item; inventory.count = 1; }
    void provide(TileCoord tile, const BlockMap& map);
};

struct WallBlock : Block {
    t1_derived BlockType getType() const noexcept final { return BlockType::wall; }
};

#undef t1_derived
