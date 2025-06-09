#pragma once
#include <memory>
#include <vector>
#include "ui_node.hpp"

class Layout : public Node {
    std::vector<std::unique_ptr<Node>> contents;
    const float margin = 5.0f;
public:
    Layout(const PixelCoord size, const PixelCoord position, const Align align) :
        Node(size, position, align) { }
    //
    void addNode(Node* node);
    void arrange();
    void draw() override final;
};
