#pragma once
#include <type_traits>
#include "config.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "ui_context.hpp"
#include "ui_defs.hpp"

namespace tin { class Data; }
class Renderer;

START_NAMESPACE_MINGUI

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
    PixelCoord getPosition() const noexcept { return position; }
    PixelCoord getSize() const noexcept { return size; }
    virtual void setPosition(const PixelCoord position) { this->position = position; }
    void setSize(const PixelCoord size) { this->size = size; }
    void setColor(const UIColor color) { this->color = color; }
    bool containsMouse(const PixelCoord mousePosition) const noexcept;
private:
    MINGUI_DISABLE_COPY_AND_MOVE(Node)
};

template<typename T>
concept DerivedFromNode = std::is_base_of_v<Node, T>;

END_NAMESPACE_MINGUI
