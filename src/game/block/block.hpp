#pragma once
#include <cstdint>
#include "config.hpp"
#include "engine/coords/tile_coord.hpp"
#include "item_stack.hpp"

using BlockType = uint16_t;

class Block {
    //Sprite shadow;
    //Sprite base;
    const BlockType type;
    const TileCoord tile;
public:
    Block(const BlockType type, const TileCoord tile) : type(type), tile(tile) { }
    virtual ~Block() = default;

    t1_finline BlockType getType() const noexcept { return type; }
    t1_finline TileCoord getTile() const noexcept { return tile; }
};

class Storage : public Block {
    Inventory inventory;
public:
    Storage(const BlockType type, const TileCoord tile) : Block(type, tile) { }
    virtual ~Storage() = default;
    //
    t1_finline Inventory& getInventory() { return inventory; }
};

class BlockTurret : public Storage {
public:
    BlockTurret(const BlockType type, const TileCoord tile) : Storage(type, tile) { }
    virtual ~BlockTurret() = default;
    //
    virtual void shoot() = 0;
};

