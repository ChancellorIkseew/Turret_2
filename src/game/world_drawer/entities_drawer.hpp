#pragma once

struct MobSoA;
struct ShellSoA;
class Camera;

class EntitiesDrawer {
public:
    EntitiesDrawer() = default;
    void draw(const Camera& camera, const MobSoA& mobs, const ShellSoA& shells, const float tickOfset);
};
