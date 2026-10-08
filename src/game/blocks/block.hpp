#pragma once
#include "engine/assets/preset_id.hpp"
#include "engine/coords/tile_coord.hpp"
#include "engine/render/texture_rect.hpp"
#include "game/common/physics_base.hpp"

class BlocksDrawer;
class BlockMap;
class Presets;
class Renderer;
class Team;
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
    impact_drill,
    factory,
    junction,
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
    BlockType getType() const noexcept final { return master->getType(); }
    BlockRot getRotation() const noexcept final { return master->getRotation(); }
    //
    void draw(BlocksDrawer& blockDrawer, Renderer& renderer, TileCoord tile) final;
    bool canAccept(ItemPresetID item, BlockRot srcRot) final { return master->canAccept(item, srcRot); };
    void accept(ItemPresetID item, BlockRot srcRot) final { master->accept(item, srcRot); };
};
