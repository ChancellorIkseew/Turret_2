#pragma once
#include <string>
#include "MINGUI/widgets/selector.hpp"

class App;

using namespace mingui;

class FrSaves final : public Selector {
    std::string targetFolder;
public:
    FrSaves() : Selector(Orientation::vertical) {
        update();
    }

    void deleteWorld();
    void saveWorld(App& app, const std::string& folder);
    void loadWorld(App& app) const;
private:
    void update();
};
