#include "transforms.hpp"

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
bool t1::areCloser(const PixelCoord first, const PixelCoord second, const float distance) {
    return abs(first.x - second.x) < distance && abs(first.y - second.y) < distance;
}
