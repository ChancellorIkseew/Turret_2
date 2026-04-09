#pragma once
#include <cstdint>

struct MobSoA;
struct ShellSoA;
class BlockMap;
class Camera;
class Presets;
class Renderer;

class EntitiesDrawer {
public:
    EntitiesDrawer() = default;
    void draw(const Camera& camera, Renderer& renderer, BlockMap& blocks,
        MobSoA& mobs, const ShellSoA& shells, const Presets& presets, const uint64_t tickCount);
};
