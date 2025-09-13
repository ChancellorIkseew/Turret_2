#pragma once
#include <array>
#include "engine/coords/tile_coord.hpp"
#include "game/world/world_map.hpp"
#include "hash_noise.hpp"

class SpotGenerator2D {
	HashNoise2D hashNoise;
public:
	SpotGenerator2D(const uint64_t seed) : hashNoise(seed) { }

	void generateSpot(WorldMap& map, const TileCoord start, const uint8_t tileType, const int spotSize) const {
		TileCoord tile = start;
		for (int s = 0; s < spotSize; ++s) {
			tile = tile + spotPreset[hashNoise.createTile(tile.x ^ s, tile.y ^ s) % 4];
			for (int i = 0; i < 5; ++i) {
				if (map.tileExists(tile + spotPreset[i]))
					map.placeOverlay(tile + spotPreset[i], tileType);
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
};
