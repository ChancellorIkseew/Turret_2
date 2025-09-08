#include "map_drawer.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/render/atlas.hpp"
#include "game/events/events.hpp"
#include "game/world/camera.hpp"
#include "game/world/world.hpp"

constexpr PixelCoord BLENDING_AREA(4.0f, 4.0f);

MapDrawer::MapDrawer(const Camera& camera, const World& world) :
    camera(camera), map(world.getMap()) {
    for (const auto& [name, id] : world.getMap().getContent().floorTypes) {
        floor.emplace(id, std::vector<PixelCoord>());
        floorTextures.emplace(id, Texture(name));
    }
    for (const auto& [name, id] : world.getMap().getContent().overlayTypes) {
        overlay.emplace(id, std::vector<PixelCoord>());
        overlayTextures.emplace(id, Texture(name));
    }
}

void MapDrawer::cacheLayers() {
    for (auto& [_type, layer] : floor) {
        layer.clear();
    }
    //
    for (int y = cashedStart.y; y < cashedEnd.y; ++y) {
        for (int x = cashedStart.x; x < cashedEnd.x; ++x) {
            floor.at(map.at(x, y).floor).push_back(t1::pixel(x, y));
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
    for (const auto& [tileType, layer] : floor) {
        sprite.setTexture(floorTextures.at(tileType));
        for (const auto& pixelCoord : layer) {
            sprite.setPosition(pixelCoord - viewCorrection);
            sprite.drawFast();
        }
    }
    //
    sprite.setSize(PixelCoord(32, 32));
    for (const auto& [tileType, layer] : overlay) {
        sprite.setTexture(overlayTextures.at(tileType));
        for (const auto& pixelCoord : layer) {
            sprite.setPosition(pixelCoord - camera.getPosition());
            sprite.drawFast();
        }
    }
    //
    drawStructures();
}

void MapDrawer::drawStructures() {
    const TileCoord start = camera.getStartTile();
    const TileCoord end = camera.getEndTile();
    for (int y = cashedStart.y; y < cashedEnd.y; ++y) {
        for (int x = cashedStart.x; x < cashedEnd.x; ++x) {
            if (map.at(x, y).block)
                map.at(x, y).block->draw();
        }
    }
}

void MapDrawer::cacheOres() {
    for (auto& [_type, layer] : overlay) {
        layer.clear();
    }
    //
    for (int y = cashedStart.y; y < cashedEnd.y; ++y) {
        for (int x = cashedStart.x; x < cashedEnd.x; ++x) {
            if (map.at(x, y).overlay != 0)
                overlay.at(map.at(x, y).overlay).push_back(t1::pixel(x, y));
        }
    }
}
