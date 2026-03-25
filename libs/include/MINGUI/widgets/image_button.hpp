#pragma once
#include <memory>
#include <functional>
#include "ui_node.hpp"

START_NAMESPACE_MINGUI

struct TextureBridge;

class ImageButton : public Node {
    std::function<void()> action;
    std::unique_ptr<TextureBridge> texture;
public:
    ImageButton(const Point size, TextureBridge* texture) :
        Node(size), texture(texture) { }
    ~ImageButton() final = default;
    //
    void draw(RenderQueue& queue) final;
    void callback(UIContext& context) final;
    void addCallback(std::function<void()> action) {
        this->action = action;
    }
};

END_NAMESPACE_MINGUI
