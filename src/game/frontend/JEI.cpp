#include "frontend.hpp"
//
#include "engine/widgets/button.hpp"
#include "game/world/world.hpp"

constexpr PixelCoord BTN_SIZE(32.0f, 32.0f);

std::unique_ptr<Container> frontend::initJEI(TileType& tileType) {
    auto jei = std::make_unique<Container>(Align::right | Align::down, Orientation::vertical);
    auto line1 = std::make_unique<Layout>(Orientation::horizontal);
    auto line2 = std::make_unique<Layout>(Orientation::horizontal);

    auto btn1 = std::make_unique<Button>(BTN_SIZE, U"1");
    auto btn2 = std::make_unique<Button>(BTN_SIZE, U"2");
    auto btn3 = std::make_unique<Button>(BTN_SIZE, U"3");
    auto btn4 = std::make_unique<Button>(BTN_SIZE, U"4");

    auto btn5 = std::make_unique<Button>(BTN_SIZE, U"5");
    auto btn6 = std::make_unique<Button>(BTN_SIZE, U"6");
    auto btn7 = std::make_unique<Button>(BTN_SIZE, U"7");
    auto btn8 = std::make_unique<Button>(BTN_SIZE, U"8");

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
    line2->addNode(btn7.release());
    line2->addNode(btn8.release());

    jei->addNode(line1.release());
    jei->addNode(line2.release());
    jei->arrange();
    return jei;
}