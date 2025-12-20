#include "chunk_grid.hpp"
//
#include "mob_manager.hpp"

using namespace t1_cgp;

static t1_finline IntPoint toIntPoint(const PixelCoord pixelCoord) {
    return IntPoint(static_cast<int>(pixelCoord.x), static_cast<int>(pixelCoord.y));
}

ChunkGrid::ChunkGrid(const TileCoord mapSize) { // move to cpp
    gridSize = (mapSize + CHUNK_SIZE - TileCoord(1, 1)) / CHUNK_SIDE_SIZE;
    chunksFind.assign(static_cast<size_t>(gridSize.x * gridSize.y), INVALID_CHUNK_ADDR);
}

void ChunkGrid::update(const MobSoA& soa) {
    for (const auto index : activeChunksIndexes) {
        chunksFind[index] = INVALID_CHUNK_ADDR;
    }
    activeChunksIndexes.clear();
    chunks.clear();

    for (size_t i = 0; i < soa.id.size(); ++i) {
        IntPoint chunk = toIntPoint(soa.position[i]) / (CHUNK_SIDE_SIZE * TILE);
        chunk.x = std::clamp(chunk.x, 0, gridSize.x - 1);
        chunk.y = std::clamp(chunk.y, 0, gridSize.y - 1);
        ChunkAddrIndex addrIndex = getAddrIndex(chunk);

        if (chunksFind[addrIndex] == INVALID_CHUNK_ADDR) { // Create new chunk.
            chunksFind[addrIndex] = static_cast<ChunkAddrIndex>(chunks.size());
            activeChunksIndexes.push_back(addrIndex);
            chunks.emplace_back(chunk);
        }

        chunks[chunksFind[addrIndex]].mobIndexes.push_back(i);
    }
}
