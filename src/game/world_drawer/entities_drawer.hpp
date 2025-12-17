#pragma once

class Camera;
class TeamsPool;

class EntitiesDrawer {
public:
    EntitiesDrawer() = default;
    void draw(const Camera& camera, const TeamsPool& teams, const float tickOfset);
};
