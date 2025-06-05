#include "transforms.hpp"

// pixel_to_tile
int t1::tile(const float pixelCoord) {
    return int(static_cast<int>(pixelCoord) / _TILE_);
}
int t1::tile(const int pixelCoord) {
    return int(pixelCoord / _TILE_);
}
TileCoord t1::tile(const float pixelCoordX, const float pixelCoordY) {
    return { tile(pixelCoordX) , tile(pixelCoordY) };
}
TileCoord t1::tile(const int pixelCoordX, const int pixelCoordY) {
    return { tile(pixelCoordX) , tile(pixelCoordY) };
}
TileCoord t1::tile(const PixelCoord pixelCoord) {
    return { tile(pixelCoord.x) , tile(pixelCoord.y) };
}

// tile_to_pixel
int t1::pixel(const int tileCoord) {
    return tileCoord * _TILE_ + _HALF_TILE_;
}
float t1::pixelF(const int tileCoord) {
    return static_cast<float>(tileCoord * _TILE_ + _HALF_TILE_);
}
PixelCoord t1::pixel(const int tileCoordX, const int tileCoordY) {
    return { pixelF(tileCoordX),  pixelF(tileCoordY) };
}
PixelCoord t1::pixel(const TileCoord tileCoord) {
    return { pixelF(tileCoord.x), pixelF(tileCoord.y) };
}

// simple math
int t1::pow2i(const int value) {
    return value * value;
}
float t1::pow2f(const float value) {
    return value * value;
}
bool t1::areCloser(const PixelCoord first, const PixelCoord second, const float distance) {
    return abs(first.x - second.x) < distance && abs(first.y - second.y) < distance;
}

// angles
float t1::radToDegree(const float radian) {
    return 180.0f - radian * 57.3f;
}
