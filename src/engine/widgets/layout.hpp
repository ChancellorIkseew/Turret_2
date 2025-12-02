#pragma once
#include <memory>
#include <vector>
#include "ui_defs.hpp"
#include "ui_node.hpp"

class Layout : public Node {
    const float margin = 5.0f;
    Orientation orientation;
protected:
    std::vector<std::shared_ptr<Node>> contents;
public:
    Layout(const Orientation orientation) :
        Node(), orientation(orientation) { }
    virtual ~Layout() override = default;
    //
    template<DerivedFromNode T>
    T* addNode(T* node) {
        contents.emplace_back(node);
        return node;
    }
    void arrange();
    void draw() final;
    void clear() { contents.clear(); }
    void setPosition(const PixelCoord position) final {
        Node::setPosition(position);
        arrange();
    }
    void translate(const tin::Data& translations) final {
        for (auto& it : contents) {
            it->translate(translations);
        }
    }
    void callback() override {
        for (auto& it : contents) {
            it->callback();
        }
    }
    const auto& getContents() const { return contents; }
private:
    void arrangeVertical();
    void arrangeHorizontal();
};
