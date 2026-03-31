#pragma once
#include <memory>
#include <MINGUI/render/render_bridge.hpp>
#include <MINGUI/widgets/ui_node.hpp>

START_NAMESPACE_MINGUI

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
    //
    void setTexture(TextureBridge* texture) {
        this->texture.reset(texture);
    }
};

END_NAMESPACE_MINGUI
