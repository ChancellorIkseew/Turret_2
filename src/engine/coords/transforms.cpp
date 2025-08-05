#include "transforms.hpp"
//
//#ifdef _WIN32
//    #include <utility>
//#elif defined(__linux__) || defined(__unix__)
//    #include <algorithm>
//#endif
#include <algorithm>
#include <utility>


// pixel_to_tile
int t1::tile(const float pixelCoord) {
    return static_cast<int>(pixelCoord) / TILE;
}
int t1::tile(const int pixelCoord) {
    return pixelCoord / TILE;
}
TileCoord t1::tile(const float pixelCoordX, const float pixelCoordY) {
    return TileCoord(tile(pixelCoordX) , tile(pixelCoordY));
}
TileCoord t1::tile(const int pixelCoordX, const int pixelCoordY) {
    return TileCoord(tile(pixelCoordX) , tile(pixelCoordY));
}
TileCoord t1::tile(const PixelCoord pixelCoord) {
    return TileCoord(tile(pixelCoord.x) , tile(pixelCoord.y));
}

// tile_to_pixel
int t1::pixel(const int tileCoord) {
    return tileCoord * TILE;
}
float t1::pixelF(const int tileCoord) {
    return static_cast<float>(tileCoord * TILE);
}
PixelCoord t1::pixel(const int tileCoordX, const int tileCoordY) {
    return PixelCoord(pixelF(tileCoordX),  pixelF(tileCoordY));
}
PixelCoord t1::pixel(const TileCoord tileCoord) {
    return PixelCoord(pixelF(tileCoord.x), pixelF(tileCoord.y));
}

// tile_centre
int t1::tileCentre(const int tileCoord) {
    return tileCoord * TILE + HALF_TILE;
}
float t1::tileCentreF(const int tileCoord) {
    return static_cast<float>(tileCoord * TILE + HALF_TILE);
}
PixelCoord t1::tileCentre(const int tileCoordX, const int tileCoordY) {
    return PixelCoord(tileCentreF(tileCoordX), tileCentreF(tileCoordY));
}
PixelCoord t1::tileCentre(const TileCoord tileCoord) {
    return PixelCoord(tileCentreF(tileCoord.x), tileCentreF(tileCoord.y));
}

// simple math
int t1::pow2i(const int value) {
    return value * value;
}
float t1::pow2f(const float value) {
    return value * value;
}
int t1::ceilUpPow2(int value) {
    --value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    ++value;
    return std::max(value, 1);
}
bool t1::areCloser(const PixelCoord first, const PixelCoord second, const float distance) {
    return abs(first.x - second.x) < distance && abs(first.y - second.y) < distance;
}

// angles
float t1::radToDegree(const float radian) {
    return 180.0f - radian * 57.3f;
}
