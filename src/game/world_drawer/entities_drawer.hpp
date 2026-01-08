#pragma once

struct MobSoA;
struct ShellSoA;
class Camera;
class Renderer;

class EntitiesDrawer {
public:
    EntitiesDrawer() = default;
    void draw(const Camera& camera, const Renderer& renderer, MobSoA& mobs, const ShellSoA& shells);
};
