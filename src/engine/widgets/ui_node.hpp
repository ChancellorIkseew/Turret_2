#pragma once
#include <type_traits>
#include "config.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "ui_context.hpp"
#include "ui_defs.hpp"

namespace tin { class Data; }
class Renderer;

class Node {
    UIColor color = UIColor::non_interactive;
    PixelCoord size, position;
public:
    Node(const PixelCoord size, const PixelCoord position) : size(size), position(position) { }
    Node(const PixelCoord size, const UIColor color)       : size(size), color(color) { }
    Node(const PixelCoord size)                            : size(size) { }
    Node() = default;
    virtual ~Node() = default;
    //
    virtual void draw(const Renderer& renderer);
    virtual void translate(const tin::Data& translations) { }
    virtual void callback(UIContext& context) = 0;
    //
    PixelCoord getPosition() const { return position; }
    PixelCoord getSize() const { return size; }
    virtual void setPosition(const PixelCoord position) { this->position = position; }
    void setSize(const PixelCoord size) { this->size = size; }
    void setColor(const UIColor color) { this->color = color; }
    bool containsMouse(const Input& input) const;
private:
    t1_disable_copy_and_move(Node)
};

template<typename T>
concept DerivedFromNode = std::is_base_of_v<Node, T>;
