#include "chunk_grid.hpp"
//
#include "mob_manager.hpp"

void ChunkGrid::update(const MobSoA& soa) {
    chunkOffsets.fill(0);

    for (const Hitbox hitbox : soa.hitbox) {
        const PixelCoord start = hitbox.getStart();
        const PixelCoord end = hitbox.getEnd();

        uint32_t x1 = static_cast<uint32_t>(start.x) >> CELL_SHIFT;
        uint32_t y1 = static_cast<uint32_t>(start.y) >> CELL_SHIFT;
        uint32_t x2 = static_cast<uint32_t>(end.x) >> CELL_SHIFT;
        uint32_t y2 = static_cast<uint32_t>(end.y) >> CELL_SHIFT;

        for (uint32_t y = y1; y <= y2; ++y) {
            for (uint32_t x = x1; x <= x2; ++x) {
                chunkOffsets[getHash(x, y)]++;
            }
        }
    }

    uint32_t currentOffset = 0;
    for (uint32_t i = 0; i < TABLE_SIZE; ++i) {
        uint32_t mobCountInChunk = chunkOffsets[i];
        chunkOffsets[i] = currentOffset;
        currentOffset += mobCountInChunk;
    }
    chunkOffsets[TABLE_SIZE] = currentOffset;
    mobIndices.resize(currentOffset);
    std::array<uint32_t, TABLE_SIZE + 1> writePtrs = chunkOffsets;

    for (uint32_t i = 0; i < static_cast<uint32_t>(soa.mobCount); ++i) {
        const Hitbox hitbox = soa.hitbox[i];
        const PixelCoord start = hitbox.getStart();
        const PixelCoord end = hitbox.getEnd();

        uint32_t x1 = static_cast<uint32_t>(start.x) >> CELL_SHIFT;
        uint32_t y1 = static_cast<uint32_t>(start.y) >> CELL_SHIFT;
        uint32_t x2 = static_cast<uint32_t>(end.x) >> CELL_SHIFT;
        uint32_t y2 = static_cast<uint32_t>(end.y) >> CELL_SHIFT;

        for (uint32_t y = y1; y <= y2; ++y) {
            for (uint32_t x = x1; x <= x2; ++x) {
                uint32_t hash = getHash(x, y);
                mobIndices[writePtrs[hash]++] = i;
            }
        }
    }
}
