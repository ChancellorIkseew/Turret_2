#include "map_drawer.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/render/atlas.hpp"
#include "camera.hpp"

constexpr PixelCoord BLENDING_AREA(4.0f, 4.0f);

MapDrawer::MapDrawer(const Camera& camera, const WorldMap& map) :
    camera(camera), map(map) {
    layers.emplace(TileType::MAGMA, std::vector<PixelCoord>());
    layers.emplace(TileType::ROCK, std::vector<PixelCoord>());
    layers.emplace(TileType::SOIL, std::vector<PixelCoord>());
    layers.emplace(TileType::WATER, std::vector<PixelCoord>());
    layers.emplace(TileType::ICE, std::vector<PixelCoord>());
    layers.emplace(TileType::SNOW, std::vector<PixelCoord>());
    sprite.setSize(PixelCoord(40, 40));
}

void MapDrawer::cacheLayers() {
    for (auto& [type, layer] : layers) {
        layer.clear();
    }
    //
    for (int y = cashedStart.y; y < cashedEnd.y; ++y) {
        for (int x = cashedStart.x; x < cashedEnd.x; ++x) {
            layers.at(map[x][y].tileType).push_back(t1::pixel(x, y));
        }
    } 
}

void MapDrawer::draw() {
    const TileCoord start = camera.getStartTile();
    const TileCoord end = camera.getEndTile();
    if (cashedStart != start || cashedEnd != end) {
        cashedStart = start;
        cashedEnd = end;
        cacheLayers();
    }
    const PixelCoord viewCorrection = camera.getPosition() + BLENDING_AREA;
    //
    for (const auto& [tileType, layer] : layers) {
        switch (tileType) {
        case TileType::MAGMA: sprite.setTexture(Texture("magma")); break;
        case TileType::ROCK: sprite.setTexture(Texture("stone")); break;
        case TileType::SOIL: sprite.setTexture(Texture("soil")); break;
        case TileType::WATER: sprite.setTexture(Texture("water")); break;
        case TileType::ICE: sprite.setTexture(Texture("ice")); break;
        case TileType::SNOW: sprite.setTexture(Texture("snow")); break;
        }
        for (const auto& pixelCoord : layer) {
            sprite.setPosition(pixelCoord - viewCorrection);
            sprite.drawFast();
        }
    }
}
