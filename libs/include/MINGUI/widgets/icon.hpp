#pragma once
#include <memory>
#include "ui_node.hpp"

START_NAMESPACE_MINGUI

struct TextureBridge;

class Icon : public Node {
    std::unique_ptr<TextureBridge> texture;
public:
    Icon(const Point size, TextureBridge* texture) :
        Node(size), texture(texture) {
    }
    ~Icon() override = default;
    //
    void draw(RenderQueue& queue) override;
    void callback(UIContext& context) override { }
};

END_NAMESPACE_MINGUI
