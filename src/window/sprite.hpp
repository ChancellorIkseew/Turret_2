#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "texture.hpp"

class Sprite {
    static inline SDL_Renderer* renderer;
    Texture texture;
    SDL_FRect rect;
    int width_ = 0;
    int height_ = 0;
    SDL_FPoint centre = SDL_FPoint(50, 50);
    double angle = 45;
public:
    Sprite(const Texture& texture) : texture(texture) {
        rect = SDL_FRect(100, 100, 100, 100);
    }

    void drawFast() {
        SDL_RenderTexture(renderer, texture.rawSDL(), nullptr, &rect);
    }

    void draw() {
        SDL_RenderTextureRotated(renderer, texture.rawSDL(),
            nullptr, &rect, angle, &centre, SDL_FlipMode::SDL_FLIP_NONE);
    }

    void setPosition(float x, float y) {
        rect.x = x;
        rect.y = y;
    }

    static void setRenderer(SDL_Renderer* renderer) {
        Sprite::renderer = renderer;
    }
};