#pragma once

class BlockMap;
class Camera;
class Assets;

class Renderer;

namespace blocks {
    void drawBlocks(const BlockMap& blocks, const Assets& assets, const Camera& camera, Renderer& renderer);
}
