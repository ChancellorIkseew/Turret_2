#pragma once
#include <SDL3/SDL.h>
#include "atlas.hpp"
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
    __forceinline void drawFast() const {
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
    __forceinline void setPosition(const float x, const float y) {
        rect.x = x;
        rect.y = y;
    }
    __forceinline void setPosition(const PixelCoord pos) {
        setPosition(pos.x, pos.y);
    }
    void setSize(const PixelCoord size) {
        rect.w = size.x;
        rect.h = size.y;
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
    static void setTranslation(const PixelCoord translation) {
        Sprite::translation = translation;
    }
};
