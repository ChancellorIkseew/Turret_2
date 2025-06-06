#include "camera.hpp"
//
#include <algorithm>
#include "coords/transforms.hpp"
#include "window/input/input.hpp"
#include "window/window.hpp"
#include <iostream>

constexpr float MIN_MAP_SCALE = 0.5f, MAX_MAP_SCALE = 5.0f;
constexpr float SCALE_FACTOR = 1.2f;
constexpr float MOTION_SPEED_MODIFIER = 20.0f;
constexpr TileCoord MAX_MAP_STRUCTURE_SIZE(6, 6);

Camera::Camera(const TileCoord mapSize) : mapScale(MIN_MAP_SCALE),
    tileMapSize(mapSize), pixelMapSize(t1::pixel(mapSize)) { }

void Camera::interact(const MainWindow& mainWindow) {
    scale();
    moveByMouse();
    moveByWASD();
    avoidEscapeFromMap();
    resize(mainWindow);
    updateMapRegion(mainWindow);
}

void Camera::moveByMouse() {
    if (Input::jactive(BindName::MidMB))
        movingStartMouseCoord = fromScreenToMap(Input::getMouseCoord());
    else if (Input::active(BindName::MidMB)) {
        const PixelCoord delta = movingStartMouseCoord - fromScreenToMap(Input::getMouseCoord());
        cameraCentre = cameraCentre + delta;
    }
}

void Camera::moveByWASD() {
    PixelCoord delta(0.0f, 0.0f);

    if (Input::active(BindName::Move_up))
        delta.y -= 1.0f;
    if (Input::active(BindName::Move_left))
        delta.x -= 1.0f;
    if (Input::active(BindName::Move_down))
        delta.y += 1.0f;
    if (Input::active(BindName::Move_right))
        delta.x += 1.0f;

    if (delta != PixelCoord(0.0f, 0.0f))
        cameraCentre = cameraCentre + (delta * MOTION_SPEED_MODIFIER / mapScale);
}

void Camera::avoidEscapeFromMap() {
    cameraCentre.x = std::clamp(cameraCentre.x, 0.0f, pixelMapSize.x);
    cameraCentre.y = std::clamp(cameraCentre.y, 0.0f, pixelMapSize.y);
}

void Camera::scale() {
    switch (Input::getMouseWheelScroll())
    {
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

void Camera::resize(const MainWindow& mainWindow) {
    cameraUpperLeftCorner = cameraCentre - mainWindow.getSize() / 2.0f / mapScale;
}

void Camera::updateMapRegion(const MainWindow& mainWindow) {
    endTile = t1::tile(fromScreenToMap(mainWindow.getSize())) + TileCoord(1, 1);
    startTile = t1::tile(fromScreenToMap(PixelCoord(0.0f, 0.0f)));
    buildingsStartTile = startTile - MAX_MAP_STRUCTURE_SIZE;
    // Correction is needed to correct big_buildings drawing.

    if (startTile.x < 0)
        startTile.x = 0;
    if (startTile.y < 0)
        startTile.y = 0;

    if (buildingsStartTile.x < 0)
        buildingsStartTile.x = 0;
    if (buildingsStartTile.y < 0)
        buildingsStartTile.y = 0;

    if (endTile.x > tileMapSize.x)
        endTile.x = tileMapSize.x;
    if (endTile.y > tileMapSize.y)
        endTile.y = tileMapSize.y;
}

PixelCoord Camera::fromMapToScreen(const PixelCoord mapCoord) const {
    return (mapCoord - cameraUpperLeftCorner) * mapScale;
}
PixelCoord Camera::fromScreenToMap(const PixelCoord screenCoord) const {
    return cameraUpperLeftCorner + screenCoord / mapScale;
}
