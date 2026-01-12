#pragma once
#include "config.hpp"
#include "engine/coords/math.hpp"
#include "engine/coords/transforms.hpp"

class Input;

class Camera {
    static constexpr float VISUAL_ARTIFACTS_START_SCALE = 0.68f;
    PixelCoord cameraUpperLeftCorner, cameraCenter;
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
    void setCenter(const PixelCoord position) { cameraCenter = position; }

    ///@brief applies correction for building max size
    TileCoord getBuildingsStartTile() const noexcept { return buildingsStartTile; }
    TileCoord getStartTile() const noexcept { return startTile; }
    TileCoord getEndTile() const noexcept { return endTile; }
    PixelCoord getCenter() const noexcept { return cameraCenter; }
    float getMapScale() const noexcept { return mapScale; }

    PixelCoord fromMapToScreen(const PixelCoord mapCoord) const noexcept;
    PixelCoord fromScreenToMap(const PixelCoord screenCoord) const noexcept;

    t1_finline bool contains(const TileCoord tile) const noexcept {
        return t1::contains(startTile, endTile, tile);
    }
    t1_finline PixelCoord getTranslation() const noexcept {
        if (mapScale > VISUAL_ARTIFACTS_START_SCALE)
            return cameraUpperLeftCorner;
        return t1::floor(cameraUpperLeftCorner * mapScale) / mapScale;
    }
private:
    void avoidEscapeFromMap();
    void resize(const PixelCoord windowSize);
    void updateMapRegion(const PixelCoord windowSize);
};
