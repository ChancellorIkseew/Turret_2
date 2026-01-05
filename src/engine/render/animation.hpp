#pragma once
#include <SDL3/SDL_render.h>
#include "atlas.hpp"
#include "config.hpp"
#include "engine/coords/math.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "sprite.hpp"

class MainWindow;

class ASprite {
    static inline SDL_Renderer* renderer = nullptr;
    static inline PixelCoord translation;
    SDL_FRect textureRect = SDL_FRect(0.0f, 0.0f, 0.0f, 0.0f);
    SDL_FRect rect = SDL_FRect(0.0f, 0.0f, 0.0f, 0.0f);
    SDL_FPoint origin = SDL_FPoint(0.0f, 0.0f);
    double angle = 0.0;
    Uint8 frame = 0U;
public:
    ASprite() = default;
    //
    ///@brief Draw on position without angle and translation by camera.
    t1_finline void drawFast() const {
        SDL_RenderTexture(renderer, Atlas::rawSDL(), &textureRect, &rect);
    }
    void draw() const {
        SDL_FRect translatedRect = rect;
        translatedRect.x -= translation.x + origin.x;
        translatedRect.y -= translation.y + origin.y;

        const float y = textureRect.y + rect.h * (float)frame;
        SDL_FRect frameRect = SDL_FRect(textureRect.x, y, rect.w, rect.h);
        SDL_RenderTextureRotated(renderer, Atlas::rawSDL(),
            &frameRect, &translatedRect, angle, &origin, SDL_FlipMode::SDL_FLIP_NONE);
    }
    //
    t1_finline void setRotationDegree(const double angleDegree) noexcept {
        angle = angleDegree;
    }
    t1_finline void setRotationRad(const double angleRad) noexcept {
        angle = t1::radToDegree(angleRad);
    }
    t1_finline void setPosition(const PixelCoord& pos) noexcept {
        rect.x = pos.x;
        rect.y = pos.y;
    }
    t1_finline void setSize(const PixelCoord& size) noexcept {
        rect.w = size.x;
        rect.h = size.y;
    }
    t1_finline void setOrigin(const PixelCoord origin) noexcept {
        this->origin = SDL_FPoint(origin.x, origin.y);
    }
    t1_finline void setTexture(const Texture& texture) noexcept {
        textureRect = texture.rect;
    }
    t1_finline void setFrame(const Uint8 frame) noexcept {
        this->frame = frame;
    }
    //
    t1_finline PixelCoord getSize() const noexcept { return PixelCoord(rect.w, rect.h); }
    t1_finline PixelCoord getPosition() const noexcept { return PixelCoord(rect.x, rect.y); }
private:
    friend MainWindow;
    static void setRenderer(SDL_Renderer* renderer) noexcept {
        ASprite::renderer = renderer;
    }
    static void setTranslation(const PixelCoord translation) noexcept {
        ASprite::translation = translation;
    }
};
