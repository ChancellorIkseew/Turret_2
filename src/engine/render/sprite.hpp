#pragma once
#include <SDL3/SDL_render.h>
#include "atlas.hpp"
#include "config.hpp"
#include "engine/coords/pixel_coord.hpp"

class MainWindow;

class Sprite {
    static inline SDL_Renderer* renderer = nullptr;
    static inline PixelCoord translation;
    SDL_FRect textureRect;
    SDL_FRect rect = SDL_FRect(0.0f, 0.0f, 0.0f, 0.0f);
    SDL_FPoint origin = SDL_FPoint(0.0f, 0.0f);
    double angle = 0.0;
public:
    Sprite(const std::string& name) : textureRect(Atlas::at(name)) {
        rect = SDL_FRect(0, 0, 32, 32);
    }
    //
    ///@brief Draw on position without angle and translation by camera.
    t1_finline void drawFast() const {
        SDL_RenderTexture(renderer, Atlas::rawSDL(), &textureRect, &rect);
    }
    void draw() {
        SDL_FRect translatedRect = rect;
        translatedRect.x -= translation.x + origin.x;
        translatedRect.y -= translation.y + origin.y;
        SDL_RenderTextureRotated(renderer, Atlas::rawSDL(),
            &textureRect, &translatedRect, angle, &origin, SDL_FlipMode::SDL_FLIP_NONE);
    }
    //
    t1_finline void setPosition(const float x, const float y) noexcept {
        rect.x = x;
        rect.y = y;
    }
    t1_finline void setPosition(const PixelCoord pos) noexcept {
        setPosition(pos.x, pos.y);
    }
    t1_finline void setSize(const PixelCoord size) noexcept {
        rect.w = size.x;
        rect.h = size.y;
    }
    t1_finline void setRotation(const double angleDegree) noexcept {
        angle = angleDegree;
    }
    t1_finline void setOrigin(const float x, const float y) noexcept {
        origin = SDL_FPoint(x, y);
    }
private:
    friend MainWindow;
    static void setRenderer(SDL_Renderer* renderer) noexcept {
        Sprite::renderer = renderer;
    }
    static void setTranslation(const PixelCoord translation) noexcept {
        Sprite::translation = translation;
    }
};
