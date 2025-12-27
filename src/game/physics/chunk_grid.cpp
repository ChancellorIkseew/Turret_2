#include "chunk_grid.hpp"
//
#include "mob_manager.hpp"

ChunkGrid::ChunkGrid(const TileCoord mapSize) { // move to cpp
    gridSize = (mapSize + t1_cgp::CHUNK_SIZE - TileCoord(1, 1)) / t1_cgp::CHUNK_SIDE_SIZE;
    chunksFind.assign(static_cast<size_t>(gridSize.x * gridSize.y), INVALID_CHUNK_ADDR);
}

void ChunkGrid::update(const MobSoA& soa) {
    for (const auto index : activeChunksIndexes) {
        chunksFind[index] = INVALID_CHUNK_ADDR;
    }
    activeChunksIndexes.clear();
    chunks.clear();

    for (size_t i = 0; i < soa.id.size(); ++i) {
        t1_cgp::IntPoint chunk = t1::tile(soa.position[i]) / t1_cgp::CHUNK_SIDE_SIZE;;
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
