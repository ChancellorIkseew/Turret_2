#pragma once

class Camera;
class TeamsPool;
class World;

class EntitiesDrawer {
    const Camera& camera;
    const TeamsPool& teams;
public:
    EntitiesDrawer(const Camera& camera, const World& world);
    void draw(const float tickOfset);
};
