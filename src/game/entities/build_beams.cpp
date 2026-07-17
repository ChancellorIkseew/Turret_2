#include "build_beams.hpp"
//
#include "engine/coords/math.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/render/renderer.hpp"

void BuildBeamsPool::addBeam(const PixelCoord builder, const TileCoord block, const int blockSize, const uint32_t color) {
    const float xMin = t1::pixelF(block.x);
    const float yMin = t1::pixelF(block.y);
    const float xMax = xMin + (static_cast<float>(blockSize) * t1::TILE_F);
    const float yMax = yMin + (static_cast<float>(blockSize) * t1::TILE_F);

    PixelCoord corners[4] = {
        { xMin, yMin }, // down left
        { xMax, yMin }, // down right
        { xMax, yMax }, // up right
        { xMin, yMax }  // up left
    };

    struct CornerDistance {
        PixelCoord point;
        float distSq = 0.f;
    };

    CornerDistance ratedCorners[4];
    for (int i = 0; i < 4; ++i) {
        const PixelCoord d = corners[i] - builder;
        ratedCorners[i] = { corners[i], t1::pow2f(d.x) + t1::pow2f(d.y) };
    }

    // sort angles
    for (int i = 1; i < 4; ++i) {
        CornerDistance key = ratedCorners[i];
        int j = i - 1;
        while (j >= 0 && ratedCorners[j].distSq > key.distSq) {
            ratedCorners[j + 1] = ratedCorners[j];
            --j;
        }
        ratedCorners[j + 1] = key;
    }

    const PixelCoord p2 = ratedCorners[0].point;

    const PixelCoord candidateA = ratedCorners[1].point;
    const PixelCoord candidateB = ratedCorners[2].point;

    // direction vector to closest corner
    const PixelCoord facing = p2 - builder;
    const PixelCoord a = candidateA - builder;

    const float crossProduct = facing.x * a.y - facing.y * a.x;

    PixelCoord p1, p3;
    if (crossProduct > 0.0f) {
        p1 = candidateA;
        p3 = candidateB;
    }
    else {
        p1 = candidateB;
        p3 = candidateA;
    }

    bool edge1Visible = true;
    if (std::abs(p2.x - p1.x) < 0.001f) {
        bool f = std::abs(p2.x - xMax) < 0.001f;
        edge1Visible = f ? (builder.x >= xMax) : (builder.x <= xMin);
    }
    else {
        bool f = std::abs(p2.y - yMax) < 0.001f;
        edge1Visible = f ? (builder.y >= yMax) : (builder.y <= yMin);
    }

    bool edge3Visible = true;
    if (std::abs(p2.x - p3.x) < 0.001f) {
        bool f = std::abs(p2.x - xMax) < 0.001f;
        edge3Visible = f ? (builder.x >= xMax) : (builder.x <= xMin);
    }
    else {
        bool f = std::abs(p2.y - yMax) < 0.001f;
        edge3Visible = f ? (builder.y >= yMax) : (builder.y <= yMin);
    }

    if (!edge1Visible)
        p1 = builder;
    if (!edge3Visible && edge1Visible)
        p3 = builder;

    beams.emplace_back(builder, p1, p2, p3, color);
}

void BuildBeamsPool::draw(Renderer& renderer, const uint64_t tickCount) const {
    for (const auto& beam : beams) {
        const uint32_t color = (beam.color & 0xFF'FF'FF'00) | (tickCount % 256);
        renderer.drawIrregularQuad(beam.p0, beam.p1, beam.p2, beam.p3, color);
    }
}
