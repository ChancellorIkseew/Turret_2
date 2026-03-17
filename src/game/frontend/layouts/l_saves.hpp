#pragma once
#include <string>
#include "MINGUI/widgets/selector.hpp"

class Engine;

using namespace mingui;

class FrSaves final : public Selector {
    std::string targetFolder;
public:
    FrSaves() : Selector(Orientation::vertical) {
        update();
    }

    void deleteWorld();
    void saveWorld(Engine& engine, const std::string& folder);
    void loadWorld(Engine& engine) const;
private:
    void update();
};
