#pragma once
#include <functional>
#include "ui_node.hpp"

class ImageButton : public Node {
    std::function<void()> action;
public:
    ImageButton(const int sizeX, const int sizeY, std::string textureName) :
        Node(PixelCoord(sizeX, sizeY), textureName) { }
    ImageButton(const PixelCoord size, std::string textureName) :
        Node(size, textureName) { }
    //
    void callback() override final;
    void addCallback(std::function<void()> action) {
        this->action = action;
    }
};
