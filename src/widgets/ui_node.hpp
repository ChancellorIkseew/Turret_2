#pragma once
#include <SDL3/SDL.h>
#include "coords/pixel_coord.hpp"
#include "render/sprite.hpp"

class Node {
    Sprite sprite;
    PixelCoord position;
    PixelCoord size;
public:
    Node(const PixelCoord size, const PixelCoord position) :
        size(size), position(position), sprite("fill") { }
    Node(const PixelCoord size) : size(size), sprite("fill") { }
    Node() : sprite("fill") { }
    //
    virtual void draw();
    virtual void callback() = 0;
    //
    PixelCoord getPosition() const { return position; }
    PixelCoord getSize() const { return size; }
    void setPosition(const PixelCoord position) { this->position = position; }
    void setSize(const PixelCoord size) { this->size = size; }
    bool containsMouse() const;
private:
    Node(const Node& other) = delete;
    Node(Node&& other) = delete;
    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) = delete;
};
