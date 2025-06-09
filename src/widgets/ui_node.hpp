#pragma once
#include "coords/pixel_coord.hpp"
#include "ui_defs.hpp"
#include <SDL3/SDL.h>
#include "render/sprite.hpp"

class Node {
    Texture texture;
    Sprite sprite;
    PixelCoord position;
    PixelCoord size;
    Align align;
public:
    Node(const PixelCoord size, const PixelCoord position, const Align align) :
        size(size), position(position), align(align), texture("res/images/fill.png"), sprite(texture) { }
    //
    virtual void draw();
    //
    PixelCoord getPosition() const { return position; }
    PixelCoord getSize() const { return size; }
    void setPosition(const PixelCoord position) { this->position = position; }
    void setSize(const PixelCoord size) { this->size = size; }
};
