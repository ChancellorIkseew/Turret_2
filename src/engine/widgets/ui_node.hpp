#pragma once
#include "engine/coords/pixel_coord.hpp"
#include "engine/render/sprite.hpp"

namespace tin {
    class Data;
}

class Node {
    Sprite sprite;
public:
    Node(const PixelCoord size, const PixelCoord position)      : sprite("fill", size, position) { }
    Node(const PixelCoord size, const std::string& textureName) : sprite(textureName, size, PixelCoord(0, 0)) { }
    Node(const PixelCoord size)                                 : sprite("fill", size, PixelCoord(0, 0)) { }
    Node()                                                      : sprite("fill") { }
    virtual ~Node() = default;
    //
    virtual void draw();
    virtual void translate(const tin::Data& translations) { }
    virtual void callback() = 0;
    //
    PixelCoord getPosition() const { return sprite.getPosition(); }
    PixelCoord getSize() const { return sprite.getSize(); }
    virtual void setPosition(const PixelCoord position) { sprite.setPosition(position); }
    void setSize(const PixelCoord size) { sprite.setSize(size); }
    void setTexture(const std::string& textureName) { sprite.setTexture(textureName); }
    bool containsMouse() const;
private:
    Node(const Node& other) = delete;
    Node(Node&& other) = delete;
    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) = delete;
};

template<typename T>
concept DerivedFromNode = std::is_base_of_v<Node, T>;
