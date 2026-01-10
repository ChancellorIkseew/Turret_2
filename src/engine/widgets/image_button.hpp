#pragma once
#include <functional>
#include "engine/render/texture.hpp"
#include "ui_node.hpp"

class ImageButton : public Node {
    std::function<void()> action;
    Texture texture;
public:
    ImageButton(const PixelCoord size, Texture texture) :
        Node(size), texture(texture) { }
    ~ImageButton() final = default;
    //
    void draw(const Renderer& renderer) final;
    void callback(UIContext& context) final;
    void addCallback(std::function<void()> action) {
        this->action = action;
    }
};
