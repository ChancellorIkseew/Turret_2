#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include "engine/coords/transforms.hpp"

struct MobSoA;

struct Chunk {
    const int32_t* beginIt;
    const int32_t* endIt;

    constexpr const int32_t* begin() const { return beginIt; }
    constexpr const int32_t* end()   const { return endIt; }
    constexpr bool empty() const { return beginIt == endIt; }
};

class ChunkGrid {
    static constexpr int32_t TABLE_SIZE = 8192;
    static constexpr int32_t TABLE_MASK = TABLE_SIZE - 1;
    static constexpr int32_t CELL_SHIFT = 8;

    std::array<uint32_t, TABLE_SIZE + 1> chunkOffsets;
    std::vector<int32_t> mobIndices;
    std::vector<uint32_t> writePtrs;
public:
    void update(const MobSoA& soa);

    t1_finline_cxpr const Chunk getChunk(const PixelCoord position) const {
        size_t hash = getHash(position);
        uint32_t start = chunkOffsets[hash];
        uint32_t end = chunkOffsets[hash + 1];
        return Chunk{ &mobIndices[start], &mobIndices[end] };
    }
    t1_finline_cxpr const Chunk getChunk(const TileCoord position) const {
        return getChunk(t1::pixel(position));
    }
private:
    static t1_finline_cxpr int32_t getHash(const int32_t cx, const int32_t cy) {
        return ((cx * 73856093) ^ (cy * 19349663)) & TABLE_MASK;
    }
    static t1_finline_cxpr int32_t getHash(const PixelCoord position) {
        int32_t cx = static_cast<int32_t>(position.x) >> CELL_SHIFT;
        int32_t cy = static_cast<int32_t>(position.y) >> CELL_SHIFT;
        return getHash(cx, cy);
    }
};
