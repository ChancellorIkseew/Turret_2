#include "gui.hpp"
//


GUI::GUI() : layout(PixelCoord(0, 0), PixelCoord(0, 0), Align::none) {
    Node* node = new Node(PixelCoord(100, 100), PixelCoord(0, 0), Align::none);
    Node* node2 = new Node(PixelCoord(200, 200), PixelCoord(500, 0), Align::none);
    layout.addNode(node);
    layout.addNode(node2);

    layout.setPosition(PixelCoord(100, 100));
    layout.arrange();
}

void GUI::draw() {
    layout.draw();
}
