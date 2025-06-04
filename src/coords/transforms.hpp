#pragma once
#include "tile_coord.hpp"
#include "pixel_coord.hpp"

constexpr int _TILE_ = 32;
constexpr int _HALF_TILE_ = 16;

constexpr float PI = 3.14159265359f;

namespace t1
{
	int tile(const float pixelCoord);
	int tile(const int pixelCoord);
	TileCoord tile(const float pixelCoordX, const float pixelCoordY);
	TileCoord tile(const int pixelCoordX, const int pixelCoordY);
	TileCoord tile(const PixelCoord pixelCoord);

	int pixel(const int tileCoord);
	float pixelF(const int tileCoord);
	PixelCoord pixel(const int tileCoordX, const int tileCoordY);
	PixelCoord pixel(const TileCoord tileCoord);

	int pow2i(const int value);
	float pow2f(const float value);
	bool areCloser(const PixelCoord first, const PixelCoord second, const float distance);
	PixelCoord correction(const PixelCoord center, const PixelCoord point, const float angleRad);
	int spiralRangeToTileRange(const int spiralRange);

	float radToDegree(const float radian);
};