#include "map_drawer.hpp"
//
#include "engine/coords/transforms.hpp"
#include "engine/render/atlas.hpp"
#include "camera.hpp"
#include "game/events/events.hpp"

constexpr PixelCoord BLENDING_AREA(4.0f, 4.0f);

MapDrawer::MapDrawer(const Camera& camera, const World& world) :
    camera(camera), map(world.getMap()), reg(world.getContent()), teams(world.getTeams()) {
    for (const auto& [id, _name] : reg.floorTypes) {
        layers.emplace(id, std::vector<PixelCoord>());
    }
    for (const auto& [id, _name] : reg.overlayTypes) {
        ores.emplace(id, std::vector<PixelCoord>());
    }
}

void MapDrawer::cacheLayers() {
    for (auto& [_type, layer] : layers) {
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
        sprite.setTexture(reg.floorTypes.at(tileType));
        for (const auto& pixelCoord : layer) {
            sprite.setPosition(pixelCoord - viewCorrection);
            sprite.drawFast();
        }
    }
    //
    sprite.setSize(PixelCoord(32, 32));
    for (const auto& [tileType, layer] : ores) {
        sprite.setTexture(reg.overlayTypes.at(tileType));
        for (const auto& pixelCoord : layer) {
            sprite.setPosition(pixelCoord - camera.getPosition());
            sprite.drawFast();
        }
    }
    //
    drawStructures();
    drawEntities();
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
    for (auto& [_type, layer] : ores) {
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

void MapDrawer::drawEntities() {
    for (const auto& [_teamID, team] : teams) {
        team.draw(camera);
    }
}
