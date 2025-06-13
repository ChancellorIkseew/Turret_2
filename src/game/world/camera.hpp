#pragma once
#include "engine/coords/pixel_coord.hpp"
#include "engine/coords/tile_coord.hpp"

class Camera {
    PixelCoord cameraUpperLeftCorner, cameraCentre;
    PixelCoord movingStartMouseCoord;
    const PixelCoord pixelMapSize;
    const TileCoord tileMapSize;
    float mapScale;
    TileCoord buildingsStartTile;
    TileCoord startTile;
    TileCoord endTile;
public:
    Camera(const TileCoord mapSize);

    void interact(const PixelCoord windowSize);

    ///@brief applies correction for building max size
    TileCoord getBuildingsStartTile() const { return buildingsStartTile; }
    TileCoord getStartTile() const { return startTile; }
    TileCoord getEndTile() const { return endTile; }
    PixelCoord getPosition() const { return cameraUpperLeftCorner; }
    float getMapScale() const { return mapScale; }

    PixelCoord fromMapToScreen(const PixelCoord screenCoord) const;
    PixelCoord fromScreenToMap(const PixelCoord screenCoord) const;

    __forceinline bool contains(const TileCoord tile) const {
        return tile.x >= startTile.x && tile.x <= endTile.x &&
               tile.y >= startTile.y && tile.y <= endTile.y;
    }
private:
    void moveByMouse();
    void moveByWASD();
    void avoidEscapeFromMap();
    void scale();
    void resize(const PixelCoord windowSize);
    void updateMapRegion(const PixelCoord windowSize);
};
