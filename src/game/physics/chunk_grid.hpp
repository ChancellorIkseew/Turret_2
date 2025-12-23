#pragma once
#include <vector>
#include "physics_base.hpp"
#include "engine/coords/tile_coord.hpp"
#include "engine/coords/transforms.hpp"

struct MobSoA;

namespace t1_cgp {
    using IntPoint = TileCoord;
    constexpr int CHUNK_SIDE_SIZE = 50;
    constexpr TileCoord CHUNK_SIZE(CHUNK_SIDE_SIZE, CHUNK_SIDE_SIZE);
}

struct Chunk {
    std::vector<size_t> mobIndexes;
    // TODO: std::vector<size_t> shellIndexes;
    t1_cgp::IntPoint position, start, end;
    Chunk(const t1_cgp::IntPoint position) :
        position(position),
        start(position* t1_cgp::CHUNK_SIDE_SIZE),
        end(start + t1_cgp::CHUNK_SIZE) {
    }
};

class ChunkGrid {
    using ChunkAddr = uint16_t;
    using ChunkAddrIndex = ChunkAddr;
    static constexpr ChunkAddr INVALID_CHUNK_ADDR = std::numeric_limits<ChunkAddr>::max();
    //
    t1_cgp::IntPoint gridSize;
    std::vector<ChunkAddrIndex> activeChunksIndexes;
    std::vector<ChunkAddr> chunksFind;
    std::vector<Chunk> chunks;
public:
    ChunkGrid(const TileCoord mapSize);
    void update(const MobSoA& soa);
    //
    t1_finline Chunk* getChunkAt(const TileCoord tile) noexcept {
        const t1_cgp::IntPoint chunk = tile / t1_cgp::CHUNK_SIDE_SIZE;
        if (chunk.x < 0 || chunk.x >= gridSize.x || chunk.y < 0 || chunk.y >= gridSize.y)
            return nullptr;
        const ChunkAddrIndex addrIndex = getAddrIndex(chunk);
        if (chunksFind[addrIndex] == INVALID_CHUNK_ADDR)
            return nullptr;
        return &chunks[chunksFind[addrIndex]];
    }
    t1_finline Chunk* getChunkAt(const PixelCoord pixel) noexcept {
        return getChunkAt(t1::tile(pixel));
    }
    //
    const auto cbegin() const noexcept { return chunks.cbegin(); }
    const auto begin()  const noexcept { return chunks.begin(); }
    const auto cend()   const noexcept { return chunks.cend(); }
    const auto end()    const noexcept { return chunks.end(); }
private:
    t1_finline ChunkAddrIndex getAddrIndex(const t1_cgp::IntPoint chunk) const noexcept {
        return static_cast<ChunkAddrIndex>(chunk.y* gridSize.x + chunk.x);
    }
};
