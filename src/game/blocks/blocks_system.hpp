#pragma once

class BlockManager;
class Camera;
class Assets;

class Renderer;

namespace blocks {



    void drawBlocks(const BlockManager& blocks, const Assets& assets, const Camera& camera, Renderer& renderer);
}
