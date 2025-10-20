#pragma once
#include "config.hpp"
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

    void update(const PixelCoord windowSize);
    void move(const PixelCoord delta);
    void moveByMouse();
    void scale();
    void setPosition(const PixelCoord position) { cameraCentre = position; }

    ///@brief applies correction for building max size
    TileCoord getBuildingsStartTile() const { return buildingsStartTile; }
    TileCoord getStartTile() const { return startTile; }
    TileCoord getEndTile() const { return endTile; }
    PixelCoord getPosition() const { return cameraUpperLeftCorner; }
    float getMapScale() const { return mapScale; }

    PixelCoord fromMapToScreen(const PixelCoord mapCoord) const;
    PixelCoord fromScreenToMap(const PixelCoord screenCoord) const;

    t1_finline bool contains(const TileCoord tile) const noexcept {
        return tile.x >= startTile.x && tile.x <= endTile.x &&
               tile.y >= startTile.y && tile.y <= endTile.y;
    }
private:
    void avoidEscapeFromMap();
    void resize(const PixelCoord windowSize);
    void updateMapRegion(const PixelCoord windowSize);
};
