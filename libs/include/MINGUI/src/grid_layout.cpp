#include <MINGUI/widgets/grid_layout.hpp>

MINGUI

void GridLayout::arrange() {
    Layout::arrange();
    //
    float maxWidth = 0;
    for (const auto& it : contents) {
        if (it->getSize().x > maxWidth)
            maxWidth = it->getSize().x;
    }
    //
    float maxHeight = 0;
    for (const auto& it : contents) {
        if (it->getSize().y > maxHeight)
            maxHeight = it->getSize().y;
    }
    //
    for (auto& it : contents) {
        it->setSize(Point(maxWidth, maxHeight));
    }
}

void GridLayout::draw(RenderQueue& queue) {
    Node::draw(queue);
    for (const auto& layout : contents) {
        for (const auto& it : static_cast<Layout*>(layout.get())->getContents()) {
            it->draw(queue);
        }
    }
}

void GridLayout::emplaceNode(Node* node) {
    if (contents.empty())
        contents.emplace_back(new Layout(contentsOrientation));
    //
    Layout* layout = static_cast<Layout*>(contents.back().get());
    if (layout->getContents().size() >= collRowLimit) {
        layout = new Layout(contentsOrientation);
        contents.emplace_back(layout);
    }
    //
    layout->addNode(node);
}
