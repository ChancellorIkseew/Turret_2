#pragma once
#include <list>
#include "engine/coords/pixel_coord.hpp"
#include "engine/render/sprite.hpp"
#include <iostream>

enum class FlakeType {
    SMALL
};

struct Flake {
    PixelCoord position;
    uint16_t lifetime = 0U + rand()%100;
    const FlakeType type = FlakeType::SMALL;
    Flake(const PixelCoord position, const FlakeType type) :
        position(position), type(type) { }
};


PixelCoord dtsin(const uint16_t time) {
    const float x = abs(sin(static_cast<float>(time) / 100.0f));
    const float y = 1.0f - x * x;
    return PixelCoord(x, y);
}

class Weather {
    std::list<Flake> flakes;
    Sprite sprite;
public:
    void init() {
        sprite.setTexture(Texture("snowflake_1"));
        sprite.setSize(PixelCoord(30, 30));
    }

    void addFlake() {
        flakes.emplace_back(PixelCoord(0, 0), FlakeType::SMALL);
        flakes.emplace_back(PixelCoord(100, 100), FlakeType::SMALL);
        flakes.emplace_back(PixelCoord(100, 0), FlakeType::SMALL);
        flakes.emplace_back(PixelCoord(0, 100), FlakeType::SMALL);
    }

    void draw() {
        for (auto& it : flakes) {
            ++it.lifetime;
            it.position = it.position + dtsin(it.lifetime) * 1.6f - dtsin(it.lifetime + 1U) * 0.5f;
            sprite.setPosition(floorf(it.position.x), floorf(it.position.y));
            sprite.drawFast();
        }
    }
};
