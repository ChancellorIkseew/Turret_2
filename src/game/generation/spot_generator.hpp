#pragma once
#include <array>
#include "engine/coords/tile_coord.hpp"
#include "game/world/world_map.hpp"

class SpotGenerator2D {
	static constexpr std::array<TileCoord, 5> spotPreset = {
		TileCoord(0, -1),
		TileCoord(-1, 0),
		TileCoord(0, 1),
		TileCoord(1, 0),
		TileCoord(0, 0)
	};
public:
	static void generateSpot(WorldMap& map, const TileCoord start, const uint8_t tileType, const int spotSize) {
		TileCoord tile = start;
		for (int s = 0; s < spotSize; ++s) {
			tile = tile + spotPreset[(tile.x ^ tile.y) % 4];
			for (int i = 0; i < 5; ++i) {
				if (map.tileExists(tile + spotPreset[i]))
					map.placeOverlay(tile + spotPreset[i], tileType);
			}
		}
	}
};
