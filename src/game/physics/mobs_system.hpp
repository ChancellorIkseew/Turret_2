#pragma once
#include <list>

struct Mob;
class Camera;
class TeamsPool;

namespace mobs {
    void processMobs(std::list<Mob>& mobs, TeamsPool& teams);
    void drawMobs(const std::list<Mob>& mobs, const Camera& camera);
}