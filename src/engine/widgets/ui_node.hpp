#pragma once
#include "engine/coords/pixel_coord.hpp"
#include "engine/render/sprite.hpp"

namespace tin {
    class Data;
}

class Node {
    Sprite sprite;
    PixelCoord position;
    PixelCoord size;
public:
    Node(const PixelCoord size, const PixelCoord position) :
        size(size), position(position), sprite("fill") { }
    Node(const PixelCoord size, std::string& textureName) :
        size(size), sprite(textureName) { }
    Node(const PixelCoord size) : size(size), sprite("fill") { }
    Node() : sprite("fill") { }
    virtual ~Node() = default;
    //
    virtual void draw();
    virtual void translate(const tin::Data& translations) { }
    virtual void callback() = 0;
    //
    PixelCoord getPosition() const { return position; }
    PixelCoord getSize() const { return size; }
    virtual void setPosition(const PixelCoord position) { this->position = position; }
    void setSize(const PixelCoord size) { this->size = size; }
    bool containsMouse() const;
private:
    Node(const Node& other) = delete;
    Node(Node&& other) = delete;
    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) = delete;
};
