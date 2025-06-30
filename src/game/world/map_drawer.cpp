#include "map_drawer.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/render/atlas.hpp"
#include "camera.hpp"
#include "game/events/events.hpp"

constexpr PixelCoord BLENDING_AREA(4.0f, 4.0f);

MapDrawer::MapDrawer(const Camera& camera, const WorldMap& map) :
    camera(camera), map(map) {
    layers.emplace(TileType::MAGMA, std::vector<PixelCoord>());
    layers.emplace(TileType::ROCK, std::vector<PixelCoord>());
    layers.emplace(TileType::SOIL, std::vector<PixelCoord>());
    layers.emplace(TileType::WATER, std::vector<PixelCoord>());
    layers.emplace(TileType::ICE, std::vector<PixelCoord>());
    layers.emplace(TileType::SNOW, std::vector<PixelCoord>());
    //sprite.setSize(PixelCoord(40, 40));
    ores.emplace(OreType::IRON, std::vector<PixelCoord>());
    ores.emplace(OreType::COPPER, std::vector<PixelCoord>());
}

void MapDrawer::cacheLayers() {
    for (auto& [type, layer] : layers) {
        layer.clear();
    }
    //
    for (int y = cashedStart.y; y < cashedEnd.y; ++y) {
        for (int x = cashedStart.x; x < cashedEnd.x; ++x) {
            layers.at(map[x][y].floor).push_back(t1::pixel(x, y));
        }
    }
}

void MapDrawer::draw() {
    const TileCoord start = camera.getStartTile();
    const TileCoord end = camera.getEndTile();
    if (cashedStart != start || cashedEnd != end || Events::active(Event::terrain_changed)) {
        cashedStart = start;
        cashedEnd = end;
        cacheLayers();
        cacheOres();
    }
    const PixelCoord viewCorrection = camera.getPosition() + BLENDING_AREA;
    //
    sprite.setSize(PixelCoord(40, 40));
    for (const auto& [tileType, layer] : layers) {
        switch (tileType) {
        case TileType::MAGMA: sprite.setTexture(Texture("magma")); break;
        case TileType::ROCK:  sprite.setTexture(Texture("stone")); break;
        case TileType::SOIL:  sprite.setTexture(Texture("soil"));  break;
        case TileType::WATER: sprite.setTexture(Texture("water")); break;
        case TileType::ICE:   sprite.setTexture(Texture("ice"));   break;
        case TileType::SNOW:  sprite.setTexture(Texture("snow"));  break;
        }
        for (const auto& pixelCoord : layer) {
            sprite.setPosition(pixelCoord - viewCorrection);
            sprite.drawFast();
        }
    }

    sprite.setSize(PixelCoord(32, 32));
    for (const auto& [oreType, layer] : ores) {
        switch (oreType) {
        case OreType::IRON: sprite.setTexture(Texture("iron")); break;
        case OreType::COPPER: sprite.setTexture(Texture("copper")); break;
        }
        for (const auto& pixelCoord : layer) {
            sprite.setPosition(pixelCoord - camera.getPosition());
            sprite.drawFast();
        }
    }

    drawStructures();
}

void MapDrawer::drawStructures() {
    const TileCoord start = camera.getStartTile();
    const TileCoord end = camera.getEndTile();
    for (int y = cashedStart.y; y < cashedEnd.y; ++y) {
        for (int x = cashedStart.x; x < cashedEnd.x; ++x) {
            if (map[x][y].block)
                map[x][y].block->draw();
        }
    }
}

void MapDrawer::cacheOres() {
    for (auto& [type, layer] : ores) {
        layer.clear();
    }
    //
    for (int y = cashedStart.y; y < cashedEnd.y; ++y) {
        for (int x = cashedStart.x; x < cashedEnd.x; ++x) {
            if (map[x][y].overlay != 0)
                ores.at(map[x][y].overlay).push_back(t1::pixel(x, y));
        }
    }
}
