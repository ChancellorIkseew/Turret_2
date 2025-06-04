#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "texture.hpp"

class Sprite {
    static inline SDL_Renderer* renderer = nullptr;
    Texture texture;
    SDL_FRect rect = SDL_FRect(0.0f, 0.0f, 0.0f, 0.0f);
    SDL_FPoint origin = SDL_FPoint(0.0f, 0.0f);
    double angle = 0.0;
public:
    Sprite(const Texture& texture) : texture(texture) {
        rect = SDL_FRect(100, 100, 100, 100);
    }
    //
    void drawFast() {
        SDL_RenderTexture(renderer, texture.rawSDL(), nullptr, &rect);
    }
    void draw() {
        SDL_RenderTextureRotated(renderer, texture.rawSDL(),
            nullptr, &rect, angle, &origin, SDL_FlipMode::SDL_FLIP_NONE);
    }
    //
    void setPosition(const float x, const float y) {
        rect.x = x;
        rect.y = y;
    }
    void setRotation(const double angleDegree) {
        angle = angleDegree;
    }
    void setOrigin(const float x, const float y) {
        origin = SDL_FPoint(x, y);
    }
private:
    friend MainWindow;
    static void setRenderer(SDL_Renderer* renderer) {
        Sprite::renderer = renderer;
    }
};