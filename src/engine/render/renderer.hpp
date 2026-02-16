#pragma once
#include <SDL3/SDL_render.h>
#include "config.hpp"
#include "engine/coords/math.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "texture.hpp"

enum class ScaleMode : uint8_t {
    nearest  = SDL_SCALEMODE_NEAREST,
    linear   = SDL_SCALEMODE_LINEAR,
    pixelart = SDL_SCALEMODE_PIXELART
};

enum class BlendMode : uint32_t {
    none                = SDL_BLENDMODE_NONE,
    blend               = SDL_BLENDMODE_BLEND,
    blend_premultiplied = SDL_BLENDMODE_BLEND_PREMULTIPLIED,
    add                 = SDL_BLENDMODE_ADD,
    add_premultiplied   = SDL_BLENDMODE_ADD_PREMULTIPLIED,
    modulate            = SDL_BLENDMODE_MOD,
    multiply            = SDL_BLENDMODE_MUL,
    invalid             = SDL_BLENDMODE_INVALID
};

class Renderer {
    static constexpr SDL_FColor WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
    SDL_Renderer* sdlRenderer = nullptr;
    SDL_Texture* commonTexture = nullptr;
    PixelCoord translation;
public:
    Renderer(SDL_Renderer* sdlRenderer) : sdlRenderer(sdlRenderer) {
        setBlendMode(BlendMode::blend);
    }
    ~Renderer() { SDL_DestroyTexture(commonTexture); }
    //
    t1_finline void drawFast(const Texture texture, const PixelCoord position, const PixelCoord size) const noexcept {
        const SDL_FRect dstRect(position.x - translation.x, position.y - translation.y, size.x, size.y);
        SDL_RenderTexture(sdlRenderer, commonTexture, &texture.rect, &dstRect);
    }
    t1_finline void drawRect(const uint32_t color, const PixelCoord position, const PixelCoord size) const noexcept {
        const SDL_FRect dstRect(position.x - translation.x, position.y - translation.y, size.x, size.y);
        SDL_SetRenderDrawColor(sdlRenderer, (color >> 24) & 0xFF, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
        SDL_RenderFillRect(sdlRenderer, &dstRect);
    }
    void draw(const Texture texture, const PixelCoord position, const PixelCoord size,
        const PixelCoord origin, const double angleRad) const noexcept {
        const SDL_FPoint sdlOrigin(origin.x, origin.y);
        const PixelCoord dstPosition = position - origin - translation;
        const SDL_FRect dstRect(dstPosition.x, dstPosition.y, size.x, size.y);
        SDL_RenderTextureRotated(sdlRenderer, commonTexture,
            &texture.rect, &dstRect, t1::radToDegree(angleRad), &sdlOrigin, SDL_FlipMode::SDL_FLIP_NONE);
    }
    void drawAnimated(const Texture texture, const PixelCoord position, const PixelCoord size,
        const PixelCoord origin, const double angleRad, const uint8_t frame, const float frameHeight) const noexcept {
        Texture frameTexture = texture;
        frameTexture.rect.h = frameHeight;
        frameTexture.rect.y += static_cast<float>(frame) * frameHeight;
        draw(frameTexture, position, size, origin, angleRad);
    }
    void drawBatched(const float* xy, const float* uv, const int* indexes,
        const int vertexCount, const int indexCount) const noexcept {
        SDL_RenderGeometryRaw(sdlRenderer, commonTexture, xy, sizeof(float) * 2, &WHITE, 0,
            uv, sizeof(float) * 2, vertexCount, indexes, indexCount, sizeof(int));
    }
    void drawBatchedColored(const float* xy, const float* uv, const int* indexes, const SDL_FColor* colors,
        const int vertexCount, const int indexCount) const noexcept {
        SDL_RenderGeometryRaw(sdlRenderer, commonTexture, xy, sizeof(float) * 2, colors, sizeof(SDL_FColor),
            uv, sizeof(float) * 2, vertexCount, indexes, indexCount, sizeof(int));
    }
    //
    void setTranslation(const PixelCoord translation) noexcept { this->translation = translation; }
    void setScaleMode(const ScaleMode mode) noexcept {
        SDL_SetTextureScaleMode(commonTexture, static_cast<SDL_ScaleMode>(mode));
    }
    void setBlendMode(const BlendMode mode) noexcept {
        SDL_SetRenderDrawBlendMode(sdlRenderer, static_cast<SDL_BlendMode>(mode));
        SDL_SetTextureBlendMode(commonTexture, static_cast<SDL_BlendMode>(mode));
    }
    void setColorModifier(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
        SDL_SetTextureColorMod(commonTexture, r, g, b);
        SDL_SetTextureAlphaMod(commonTexture, a);
    }
    void resetColorModifier() {
        setColorModifier(255, 255, 255, 255);
    }
    void createCommonTexture(SDL_Surface* comonSurface) {
        if (commonTexture)
            SDL_DestroyTexture(commonTexture);
        commonTexture = SDL_CreateTextureFromSurface(sdlRenderer, comonSurface);
        setScaleMode(ScaleMode::nearest);
    }
private:
    t1_disable_copy_and_move(Renderer)
};
