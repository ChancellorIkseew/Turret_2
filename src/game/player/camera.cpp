#include "camera.hpp"
//
#include <algorithm>
#include "engine/window/input/input.hpp"

constexpr float MIN_MAP_SCALE = 0.25f, MAX_MAP_SCALE = 5.0f;
constexpr float SCALE_FACTOR = 1.2f;
constexpr float MOTION_SPEED = 20.0f;
constexpr TileCoord MAX_BLOCK_SIZE(3, 3);

Camera::Camera(const TileCoord mapSize, const bool inertia) : m_mapScale(MIN_MAP_SCALE),
    m_tileMapSize(mapSize), m_pixelMapSize(t1::pixel(mapSize)), m_inertia(inertia) { }

void Camera::update(const PixelCoord windowSize, const uint64_t frameDelayNs) {
    m_targetCenter.x = std::clamp(m_targetCenter.x, 0.0f, m_pixelMapSize.x);
    m_targetCenter.y = std::clamp(m_targetCenter.y, 0.0f, m_pixelMapSize.y);

    if (m_inertia) {
        constexpr float SMOOTH_FACTOR = 6.0f;
        const float deltaTime = static_cast<float>(frameDelayNs) / 1'000'000'000;
        const float interpolationFactor = 1.0f - std::exp(-SMOOTH_FACTOR * deltaTime);
        m_realCenter += (m_targetCenter - m_realCenter) * interpolationFactor;
    }
    else
        m_realCenter = m_targetCenter;
    
    resize(windowSize);
    updateMapRegion(windowSize);
}

void Camera::moveByMouse(const Input& input) {
    if (input.jactive(Drag_camera))
        m_movingStartMouseCoord = input.getMouseCoord();
    else if (input.active(Drag_camera)) {
        const PixelCoord mouseScreenCoord = input.getMouseCoord();
        m_targetCenter += (m_movingStartMouseCoord - mouseScreenCoord) / m_mapScale;
        m_movingStartMouseCoord = mouseScreenCoord;
    }
}

void Camera::move(const PixelCoord delta) {
    if (delta != PixelCoord(0.0f, 0.0f))
        m_targetCenter += (delta * MOTION_SPEED / m_mapScale);
}

void Camera::scale(const Input& input) {
    switch (input.getMouseWheelScroll()) {
    case MouseWheelScroll::none:
        return;
    case MouseWheelScroll::up:
        if (m_mapScale >= MIN_MAP_SCALE)
            m_mapScale /= SCALE_FACTOR;
        break;
    case MouseWheelScroll::down:
        if (m_mapScale <= MAX_MAP_SCALE)
            m_mapScale *= SCALE_FACTOR;
        break;
    }
    m_mapScale = std::clamp(m_mapScale, MIN_MAP_SCALE, MAX_MAP_SCALE);
}

void Camera::resize(const PixelCoord windowSize) {
    m_cameraUpperLeftCorner = m_realCenter - windowSize / 2.0f / m_mapScale;
}

void Camera::updateMapRegion(const PixelCoord windowSize) {
    m_endTile = t1::tile(fromScreenToMap(windowSize) + t1::TILE_PC * 1.5f);
    m_startTile = t1::tile(fromScreenToMap(PixelCoord(0.0f, 0.0f)) - t1::TILE_PC);
    m_buildingsStartTile = m_startTile - MAX_BLOCK_SIZE;
    // Correction is needed to correct big_blocks drawing.

    m_startTile.x = std::max(m_startTile.x, 0);
    m_startTile.y = std::max(m_startTile.y, 0);
    m_buildingsStartTile.x = std::max(m_buildingsStartTile.x, 0);
    m_buildingsStartTile.y = std::max(m_buildingsStartTile.y, 0);
    m_endTile.x = std::min(m_endTile.x, m_tileMapSize.x);
    m_endTile.y = std::min(m_endTile.y, m_tileMapSize.y);
}

PixelCoord Camera::fromMapToScreen(const PixelCoord mapCoord) const noexcept {
    return (mapCoord - m_cameraUpperLeftCorner) * m_mapScale;
}
PixelCoord Camera::fromScreenToMap(const PixelCoord screenCoord) const noexcept {
    return m_cameraUpperLeftCorner + screenCoord / m_mapScale;
}

PixelCoord Camera::getTranslation() const noexcept {
    constexpr float VISUAL_ARTIFACTS_START_SCALE = 0.68f;
    if (m_mapScale > VISUAL_ARTIFACTS_START_SCALE)
        return m_cameraUpperLeftCorner;
    return t1::floor(m_cameraUpperLeftCorner * m_mapScale) / m_mapScale;
}
