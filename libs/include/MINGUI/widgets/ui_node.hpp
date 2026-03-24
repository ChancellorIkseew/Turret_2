#pragma once
#include <type_traits>
#include "MINGUI/core/palette.hpp"
#include "MINGUI/core/ui_context.hpp"
#include "MINGUI/core/ui_defs.hpp"

START_NAMESPACE_MINGUI

class Localization;
class RenderQueue;

class Node {
    Palette palette = NULL_PALETTE;
    Point size, position;
public:
    Node(const Point size, const Point position)  : size(size), position(position) { }
    Node(const Point size)                        : size(size) { }
    Node() = default;
    virtual ~Node() = default;
    //
    virtual void draw(RenderQueue& queue);
    virtual void translate(const Localization& localization) { }
    virtual void callback(UIContext& context) = 0;
    //
    Point getPosition() const noexcept { return position; }
    Point getSize() const noexcept { return size; }
    virtual void setPosition(const Point position) { this->position = position; }
    void setSize(const Point size) noexcept { this->size = size; }
    void setPalette(const Palette& palette) noexcept { this->palette = palette; }
    const Palette& getPalette() const noexcept { return palette; }
    bool containsMouse(const Point mousePosition) const noexcept;
private:
    MINGUI_DISABLE_COPY_AND_MOVE(Node)
};

template<typename T>
concept DerivedFromNode = std::is_base_of_v<Node, T>;

END_NAMESPACE_MINGUI
