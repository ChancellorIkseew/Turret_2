#pragma once
#include "config.hpp"
#include "engine/coords/transforms.hpp"

class Input;

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
    void moveByMouse(const Input& input);
    void scale(const Input& input);
    void setPosition(const PixelCoord position) { cameraCentre = position; }

    ///@brief applies correction for building max size
    TileCoord getBuildingsStartTile() const noexcept { return buildingsStartTile; }
    TileCoord getStartTile() const noexcept { return startTile; }
    TileCoord getEndTile() const noexcept { return endTile; }
    PixelCoord getPosition() const noexcept { return cameraUpperLeftCorner; }
    float getMapScale() const noexcept { return mapScale; }

    PixelCoord fromMapToScreen(const PixelCoord mapCoord) const noexcept;
    PixelCoord fromScreenToMap(const PixelCoord screenCoord) const noexcept;

    t1_finline bool contains(const TileCoord tile) const noexcept {
        return t1::contains(startTile, endTile, tile);
    }
private:
    void avoidEscapeFromMap();
    void resize(const PixelCoord windowSize);
    void updateMapRegion(const PixelCoord windowSize);
};
