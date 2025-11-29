#include <list>
#include "block.hpp"

class Blocks {
    std::list<Block> blocks;

    const auto find(const TileCoord tile) {
        for (auto it = blocks.cbegin(); it != blocks.cend(); ++it) {
            if (tile == it->getTile())
                return it;
        }
    }
public:
    Block& getBlock(const TileCoord tile) {
        for (auto& block : blocks) {
            if (tile == block.getTile())
                return block;
        }
    }

    void addBlock(const TileCoord tile) {
        blocks.erase(find(tile));
    }


};
