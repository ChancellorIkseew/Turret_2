#pragma once
#include <array>
#include "engine/coords/tile_coord.hpp"
#include "game/world/world_map.hpp"
#include "squirell_noise.hpp"

class SpotGenerator2D {
    SquirellNoise2D squirellNoise;
public:
    SpotGenerator2D(const uint64_t seed) : squirellNoise(seed) { }
    
    void generateSpot(std::vector<uint8_t>& overlay, const TileCoord start, const uint8_t tileType,
        const int spotSize, const TileCoord mapSize) const {
        TileCoord tile = start;
        constexpr uint32_t SEED_OFFSET = 0;
        for (int i = 0; i < spotSize; ++i) {
            tile += spotPreset[squirellNoise.createTile(tile.x ^ i, tile.y ^ i, SEED_OFFSET) % 3];
            for (const TileCoord offset : spotPreset) {
                const TileCoord target = tile + offset;
                if (tileExists(target, mapSize))
                    overlay[getIndex(target, mapSize)] = tileType;
            }
        }
    }
private:
    static constexpr std::array<TileCoord, 5> spotPreset = {
        TileCoord(0, -1),
        TileCoord(-1, 0),
        TileCoord(0, 1),
        TileCoord(1, 0),
        TileCoord(0, 0)
    };

    constexpr bool tileExists(const TileCoord tile, const TileCoord mapSize) const {
        return tile.x >= 0 && tile.x < mapSize.x &&
               tile.y >= 0 && tile.y < mapSize.y;
    }
    static constexpr int getIndex(const TileCoord tile, const TileCoord mapSize) noexcept {
        return tile.x + tile.y * mapSize.x;
    }
};
