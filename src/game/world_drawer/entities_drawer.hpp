#pragma once

struct MobSoA;
struct ShellSoA;
class Camera;

class EntitiesDrawer {
public:
    EntitiesDrawer() = default;
    void draw(const Camera& camera, MobSoA& mobs, const ShellSoA& shells);
};
