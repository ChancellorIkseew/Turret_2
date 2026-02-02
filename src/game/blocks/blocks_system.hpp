#pragma once

class Blocks;
class Camera;
class Presets;

class Renderer;

namespace blocks {



    void drawBlocks(const Blocks& blocks, const Presets& presets, const Camera& camera, const Renderer& renderer);
}