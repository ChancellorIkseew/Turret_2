#include "frontend.hpp"
//
#include "engine/widgets/image_button.hpp"
#include "game/world/world.hpp"

constexpr PixelCoord BTN_SIZE(32.0f, 32.0f);

std::unique_ptr<Container> frontend::initJEI(TileType& tileType) {
    auto jei = std::make_unique<Container>(Align::right | Align::down, Orientation::vertical);
    auto line1 = std::make_unique<Layout>(Orientation::horizontal);
    auto line2 = std::make_unique<Layout>(Orientation::horizontal);

    auto btn1 = std::make_unique<ImageButton>(BTN_SIZE, "snow");
    auto btn2 = std::make_unique<ImageButton>(BTN_SIZE, "ice");
    auto btn3 = std::make_unique<ImageButton>(BTN_SIZE, "water");
    auto btn4 = std::make_unique<ImageButton>(BTN_SIZE, "soil");

    auto btn5 = std::make_unique<ImageButton>(BTN_SIZE, "stone");
    auto btn6 = std::make_unique<ImageButton>(BTN_SIZE, "magma");

    btn1->addCallback([&]() {tileType = TileType::SNOW; });
    btn2->addCallback([&]() {tileType = TileType::ICE; });
    btn3->addCallback([&]() {tileType = TileType::WATER; });
    btn4->addCallback([&]() {tileType = TileType::SOIL; });
    btn5->addCallback([&]() {tileType = TileType::ROCK; });
    btn6->addCallback([&]() {tileType = TileType::MAGMA; });

    line1->addNode(btn1.release());
    line1->addNode(btn2.release());
    line1->addNode(btn3.release());
    line1->addNode(btn4.release());

    line2->addNode(btn5.release());
    line2->addNode(btn6.release());

    jei->addNode(line1.release());
    jei->addNode(line2.release());
    jei->arrange();
    return jei;
}