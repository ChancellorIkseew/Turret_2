#pragma once
#include <functional>
#include <string>
#include "MINGUI/widgets/selector.hpp"

class App;

using namespace mingui;

class FrSaves final : public Selector {
    std::string targetFolder;
public:
    std::function<void(const std::string&)> onSelect;

    FrSaves() : Selector(Orientation::vertical) {
        refresh();
    }

    const std::string& getTargetFolder() const { return targetFolder; }
    void refresh();
};
