#pragma once
#include <cstdint>
#include <vector>
#include "engine/coords/pixel_coord.hpp"
#include "engine/coords/tile_coord.hpp"

class Renderer;

struct BuildBeam {
    PixelCoord p0, p1, p2, p3;
    uint32_t color = 0xFF'FF'FF'FF;
};

class BuildBeamsPool {
    std::vector<BuildBeam> beams;
public:
    void addBeam(const PixelCoord builder, const TileCoord block, const TileCoord blockSize, const uint32_t color);
    void clear() { beams.clear(); }
    void draw(Renderer& renderer, const uint64_t tickCount) const;
};
