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
public:
    Node(const PixelCoord size, const PixelCoord position) :
        size(size), position(position), texture("res/images/fill.png"), sprite(texture) { }
    Node(const PixelCoord size) :
        size(size), texture("res/images/fill.png"), sprite(texture) { }
    Node() : texture("res/images/fill.png"), sprite(texture) { }
    //
    virtual void draw();
    //
    PixelCoord getPosition() const { return position; }
    PixelCoord getSize() const { return size; }
    void setPosition(const PixelCoord position) { this->position = position; }
    void setSize(const PixelCoord size) { this->size = size; }

    inline bool contains(const PixelCoord mouse) {
        const PixelCoord end = position + size;
        return mouse.x >= position.x && mouse.x <= position.x &&
               mouse.y >= end.y && mouse.y <= end.y;
    }
private:
    Node(const Node& other) = delete;
    Node(Node&& other) = delete;
    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) = delete;
};
