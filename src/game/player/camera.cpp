#include "camera.hpp"
//
#include <algorithm>
#include "engine/coords/transforms.hpp"
#include "engine/window/input/input.hpp"

constexpr float MIN_MAP_SCALE = 0.25f, MAX_MAP_SCALE = 5.0f; // Strong visual artifacts with (slow motion and scale < 0.68). 
constexpr float SCALE_FACTOR = 1.2f;
constexpr float MOTION_SPEED = 20.0f;
constexpr TileCoord MAX_MAP_STRUCTURE_SIZE(6, 6);

Camera::Camera(const TileCoord mapSize) : mapScale(MIN_MAP_SCALE),
    tileMapSize(mapSize), pixelMapSize(t1::pixel(mapSize)) { }

void Camera::update(const PixelCoord windowSize) {
    avoidEscapeFromMap();
    resize(windowSize);
    updateMapRegion(windowSize);
}

void Camera::moveByMouse(const Input& input) {
    if (input.jactive(MidMB))
        movingStartMouseCoord = fromScreenToMap(input.getMouseCoord());
    else if (input.active(MidMB)) {
        const PixelCoord delta = movingStartMouseCoord - fromScreenToMap(input.getMouseCoord());
        cameraCentre = cameraCentre + delta;
    }
}

void Camera::move(const PixelCoord delta) {
    if (delta != PixelCoord(0.0f, 0.0f))
        cameraCentre = cameraCentre + (delta * MOTION_SPEED / mapScale);
}

void Camera::avoidEscapeFromMap() {
    cameraCentre.x = std::clamp(cameraCentre.x, 0.0f, pixelMapSize.x);
    cameraCentre.y = std::clamp(cameraCentre.y, 0.0f, pixelMapSize.y);
}

void Camera::scale(const Input& input) {
    switch (input.getMouseWheelScroll()) {
    case MouseWheelScroll::none:
        return;
    case MouseWheelScroll::up:
        if (mapScale >= MIN_MAP_SCALE)
            mapScale /= SCALE_FACTOR;
        break;
    case MouseWheelScroll::down:
        if (mapScale <= MAX_MAP_SCALE)
            mapScale *= SCALE_FACTOR;
        break;
    }
    mapScale = std::clamp(mapScale, MIN_MAP_SCALE, MAX_MAP_SCALE);
}

void Camera::resize(const PixelCoord windowSize) {
    cameraUpperLeftCorner = cameraCentre - windowSize / 2.0f / mapScale;
}

void Camera::updateMapRegion(const PixelCoord windowSize) {
    endTile = t1::tile(fromScreenToMap(windowSize)) + TileCoord(1, 1);
    startTile = t1::tile(fromScreenToMap(PixelCoord(0.0f, 0.0f)));
    buildingsStartTile = startTile - MAX_MAP_STRUCTURE_SIZE;
    // Correction is needed to correct big_buildings drawing.

    startTile.x = std::max(startTile.x, 0);
    startTile.y = std::max(startTile.y, 0);
    buildingsStartTile.x = std::max(buildingsStartTile.x, 0);
    buildingsStartTile.y = std::max(buildingsStartTile.y, 0);
    endTile.x = std::min(endTile.x, tileMapSize.x);
    endTile.y = std::min(endTile.y, tileMapSize.y);
}

PixelCoord Camera::fromMapToScreen(const PixelCoord mapCoord) const noexcept {
    return (mapCoord - cameraUpperLeftCorner) * mapScale;
}
PixelCoord Camera::fromScreenToMap(const PixelCoord screenCoord) const noexcept {
    return cameraUpperLeftCorner + screenCoord / mapScale;
}
