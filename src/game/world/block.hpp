#include <cstdint>
#include "config.hpp"
#include "engine/coords/tile_coord.hpp"

using BlockType = uint16_t;

class Block {
    //Sprite shadow;
    //Sprite base;
    const BlockType type;
    const TileCoord tile;
public:
    Block(const BlockType type, const TileCoord tile) : type(type), tile(tile) { }

    t1_finline BlockType getType() const noexcept { return type; }
    t1_finline TileCoord getTile() const noexcept { return tile; }
};
