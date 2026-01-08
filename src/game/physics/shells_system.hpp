#pragma once

struct MobSoA;
struct ShellSoA;
class Camera;
class Renderer;
class ShellManager;

namespace shells {
    void processShells(ShellSoA& soa, MobSoA& mobs);
    void cleanupShells(ShellManager& manager);
    void drawShells(const ShellSoA& soa, const Camera& camera, const Renderer& renderer);
}
