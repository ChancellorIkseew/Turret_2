#pragma once
#include "blocks_drawer.hpp"
#include "map_drawer.hpp"

class BlockMap;
class Camera;
class Presets;
class Renderer;
class WorldMap;

class WorldDrawer {
    MapDrawer mapDrawer;
    BlocksDrawer blocksDrawer;
public:
    WorldDrawer(const Assets& assets) : mapDrawer(assets) { }
    //
    void drawMap(const Camera& camera, Renderer& renderer, const WorldMap& map) {
        mapDrawer.draw(camera, renderer, map);
    }
    void drawBlockShadows(const BlockMap& blocks, const Camera& camera, Renderer& renderer) {
        blocksDrawer.drawShadows(blocks, camera, renderer);
    }
    void drawBlocks(const BlockMap& blocks, Renderer& renderer, const Presets& presets) {
        blocksDrawer.draw(blocks, presets, renderer);
    }
    void drawBlocksInProgress(const BlockMap& blocks, Renderer& renderer, const Presets& presets) {
        blocksDrawer.drawInProgress(blocks, presets, renderer);
    }
};
