#pragma once
#include <cstdint>
#include "config.hpp"
#include "engine/coords/math.hpp"
#include "engine/coords/transforms.hpp"

class Input;

class Camera {
    PixelCoord m_cameraUpperLeftCorner, m_realCenter, m_targetCenter;
    PixelCoord m_movingStartMouseCoord;
    const PixelCoord m_pixelMapSize;
    const TileCoord m_tileMapSize;
    float m_mapScale;
    TileCoord m_buildingsStartTile;
    TileCoord m_startTile;
    TileCoord m_endTile;
    bool m_inertia;
public:
    Camera(const TileCoord mapSize, const bool inertia);

    void update(const PixelCoord windowSize, const uint64_t frameDelayNs);
    void move(const PixelCoord delta);
    void moveByMouse(const Input& input);
    void scale(const Input& input);
    void setTargetCenter(const PixelCoord position) { m_targetCenter = position; }
    void teleport(const PixelCoord position) { m_realCenter = m_targetCenter = position; }
    void toggleInertia(const bool flag) { m_inertia = flag; }

    ///@brief applies correction for building max size
    TileCoord  getBuildingsStartTile() const noexcept { return m_buildingsStartTile; }
    TileCoord  getStartTile()          const noexcept { return m_startTile; }
    TileCoord  getEndTile()            const noexcept { return m_endTile; }
    PixelCoord getRealCenter()         const noexcept { return m_realCenter; }
    float      getMapScale()           const noexcept { return m_mapScale; }

    PixelCoord fromMapToScreen(const PixelCoord mapCoord) const noexcept;
    PixelCoord fromScreenToMap(const PixelCoord screenCoord) const noexcept;
    PixelCoord getTranslation() const noexcept;

    t1_finline bool contains(const TileCoord tile) const noexcept {
        return t1::contains(m_startTile, m_endTile, tile);
    }
    t1_finline bool contains(const PixelCoord position) const noexcept {
        return t1::contains(t1::pixel(m_startTile), t1::pixel(m_endTile), position);
    }
private:
    void resize(const PixelCoord windowSize);
    void updateMapRegion(const PixelCoord windowSize);
};
