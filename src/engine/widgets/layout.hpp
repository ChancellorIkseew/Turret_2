#pragma once
#include <memory>
#include <vector>
#include "ui_defs.hpp"
#include "ui_node.hpp"

class Layout : public Node {
    std::vector<std::unique_ptr<Node>> contents;
    const float margin = 5.0f;
    Orientation orientation;
public:
    Layout(const Orientation orientation) :
        Node(), orientation(orientation) { }
    virtual ~Layout() override = default;
    //
    void addNode(Node* node);
    void arrange();
    void draw() final;
    void setPosition(const PixelCoord position) final {
        Node::setPosition(position);
        arrange();
    }
    void callback() final {
        if (!containsMouse())
            return;
        for (auto& it : contents) {
            it->callback();
        }
    }
private:
    void arrangeVertical();
    void arrangeHorizontal();
};
