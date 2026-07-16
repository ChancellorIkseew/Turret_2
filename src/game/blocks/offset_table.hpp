#pragma once
#include <array>
#include <span>
#include "engine/coords/tile_coord.hpp"

namespace t1 {
    // 1*1 + 2*2 + 3*3 + 4*4 = 1 + 4 + 9 + 16 = 30 elements
    struct BlockOffsetsTable {
        std::array<TileCoord, 30> data{};
        std::array<uint8_t, 5> sizeOffsets{};
    };

    [[nodiscard]] constexpr BlockOffsetsTable generateOffsetsTable() noexcept {
        BlockOffsetsTable table{};
        size_t currentIndex = 0;
        //
        for (int size = 1; size <= 4; ++size) {
            table.sizeOffsets[size] = static_cast<uint8_t>(currentIndex);
            for (int dy = 0; dy < size; ++dy) {
                for (int dx = 0; dx < size; ++dx) {
                    table.data[currentIndex++] = TileCoord(dx, dy);
                }
            }
        }
        return table;
    }

    inline constexpr BlockOffsetsTable offsetsTable = generateOffsetsTable();

    [[nodiscard]] inline std::span<const TileCoord> getTileOffsets(const int size) noexcept {
        if (size <= 0 || size > 4) [[unlikely]] {
            return {};
        }

        const size_t startIndex = offsetsTable.sizeOffsets[size];
        const size_t elementCount = static_cast<size_t>(size * size);

        return std::span<const TileCoord>(&offsetsTable.data[startIndex], elementCount);
    }
}
