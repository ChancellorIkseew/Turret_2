#pragma once

struct MobSoA;
class Camera;

class EntitiesDrawer {
public:
    EntitiesDrawer() = default;
    void draw(const Camera& camera, const MobSoA& soa, const float tickOfset);
};
