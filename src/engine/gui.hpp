#pragma once
#include <memory>
#include <vector>
#include "widgets/container.hpp"

class GUI {
    std::vector<std::unique_ptr<Container>> containers;
public:
    GUI();
    void draw();
};
