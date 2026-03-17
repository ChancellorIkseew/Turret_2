#pragma once
#include <type_traits>
#include "ui_context.hpp"
#include "ui_defs.hpp"

namespace tin { class Data; }

START_NAMESPACE_MINGUI

class RenderQueue;

class Node {
    UIColor color = UIColor::non_interactive;
    Point size, position;
public:
    Node(const Point size, const Point position) : size(size), position(position) { }
    Node(const Point size, const UIColor color)  : size(size), color(color) { }
    Node(const Point size)                       : size(size) { }
    Node() = default;
    virtual ~Node() = default;
    //
    virtual void draw(RenderQueue& queue);
    virtual void translate(const tin::Data& translations) { }
    virtual void callback(UIContext& context) = 0;
    //
    Point getPosition() const noexcept { return position; }
    Point getSize() const noexcept { return size; }
    virtual void setPosition(const Point position) { this->position = position; }
    void setSize(const Point size) { this->size = size; }
    void setColor(const UIColor color) { this->color = color; }
    bool containsMouse(const Point mousePosition) const noexcept;
private:
    MINGUI_DISABLE_COPY_AND_MOVE(Node)
};

template<typename T>
concept DerivedFromNode = std::is_base_of_v<Node, T>;

END_NAMESPACE_MINGUI
