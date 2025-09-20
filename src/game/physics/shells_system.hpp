#pragma once
#include <list>

struct Shell;
class Camera;
class TeamsPool;

namespace shells {
    void processShells(std::list<Shell>& shells, TeamsPool& teams);
    void drawShells(std::list<Shell>& shells, const Camera& camera);
}
