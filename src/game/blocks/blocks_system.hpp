#pragma once

class Blocks;
class Camera;
class Assets;

class Renderer;

namespace blocks {



    void drawBlocks(const Blocks& blocks, const Assets& assets, const Camera& camera, Renderer& renderer);
}
