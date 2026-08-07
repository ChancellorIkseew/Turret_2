#include <MINGUI/widgets/layout.hpp>
//
#include <ranges>

MINGUI

void Layout::setPaletteRecursive(const Palette& palette) {
    for (const auto& node : contents) {
        if (Layout* layout = node->asLayout())
            layout->setPaletteRecursive(palette);
        else
            node->setPalette(palette);
    }
}

void Layout::arrange() {
    switch (orientation) {
    case Orientation::vertical:   arrangeVertical();   break;
    case Orientation::horizontal: arrangeHorizontal(); break;
    default:                      arrangeGrid();       break;
    }
}

void Layout::arrangeVertical() {
    float maxWidth = padding * 2.0f;
    Point position = Point(padding, padding) + getPosition();
    for (const auto& node : contents) {
        node->setPosition(position);
        position.y += margin + node->getSize().y;
        if (node->getSize().x > maxWidth)
            maxWidth = node->getSize().x;
    }
    setSize(Point(maxWidth + padding * 2.0f, position.y - getPosition().y - margin + padding));
}

void Layout::arrangeHorizontal() {
    float maxHeight = padding * 2.0f;
    Point position = Point(padding, padding) + getPosition();
    for (const auto& node : contents) {
        node->setPosition(position);
        position.x += margin + node->getSize().x;
        if (node->getSize().y > maxHeight)
            maxHeight = node->getSize().y;
    }
    setSize(Point(position.x - getPosition().x - margin + padding, maxHeight + padding * 2.0f));
}

void Layout::arrangeGrid() {
    Point maxNodeSize;
    for (const auto& node : contents) {
        maxNodeSize.x = std::max(maxNodeSize.x, node->getSize().x);
        maxNodeSize.y = std::max(maxNodeSize.y, node->getSize().y);
    }

    const Point basePos = getPosition() + Point(padding, padding);
    const size_t limit = std::max<size_t>(1, collRowLimit);
    const size_t total = contents.size();
    size_t cols = 0;
    size_t rows = 0;

    if (orientation == Orientation::vertical_grid) {
        for (const auto& [i, node] : std::views::enumerate(contents)) {
            const float x = static_cast<float>(i / limit) * (maxNodeSize.x + margin);
            const float y = static_cast<float>(i % limit) * (maxNodeSize.y + margin);
            node->setPosition(basePos + Point(x, y));
        }
        rows = std::min(total, limit);
        cols = (total + limit - 1) / limit;
    }
    if (orientation == Orientation::horizontal_grid) {
        for (const auto& [i, node] : std::views::enumerate(contents)) {
            const float x = static_cast<float>(i % limit) * (maxNodeSize.x + margin);
            const float y = static_cast<float>(i / limit) * (maxNodeSize.y + margin);
            node->setPosition(basePos + Point(x, y));
        }
        cols = std::min(total, limit);
        rows = (total + limit - 1) / limit;
    }

    const float sizeX = padding * 2.f + cols * maxNodeSize.x + std::max<size_t>(cols - 1, 0) * margin;
    const float sizeY = padding * 2.f + rows * maxNodeSize.y + std::max<size_t>(rows - 1, 0) * margin;
    setSize(Point(sizeX, sizeY));
}

void Layout::draw(RenderQueue& queue) {
    Node::draw(queue);
    for (const auto& node : contents) {
        node->draw(queue);
    }
}
