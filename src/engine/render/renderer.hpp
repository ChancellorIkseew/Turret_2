#pragma once
#include <SDL3/SDL_render.h>
#include "config.hpp"
#include "engine/coords/math.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "texture.hpp"

class Renderer {
    static constexpr SDL_FColor WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
    SDL_Renderer* sdlRenderer = nullptr;
    SDL_Texture* comonTexture = nullptr;
    PixelCoord translation;
public:
    Renderer() = default;
    //
    t1_finline void drawFast(const Texture texture, const PixelCoord position, const PixelCoord size) const noexcept {
        const SDL_FRect dstRect(position.x - translation.x, position.y - translation.y, size.x, size.y);
        SDL_RenderTexture(sdlRenderer, comonTexture, &texture.rect, &dstRect);
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
        SDL_RenderTextureRotated(sdlRenderer, comonTexture,
            &texture.rect, &dstRect, t1::radToDegree(angleRad), &sdlOrigin, SDL_FlipMode::SDL_FLIP_NONE);
    }
    void drawAnimated(const Texture texture, const PixelCoord position, const PixelCoord size,
        const PixelCoord origin, const double angleRad, const uint8_t frame, const uint8_t frameCount) const noexcept {
        Texture frameTexture = texture;
        frameTexture.rect.h /= frameCount;
        frameTexture.rect.y += static_cast<float>(frame) * frameTexture.rect.h;
        draw(frameTexture, position, size, origin, angleRad);
    }
    void drawBatched(const float* xy, const float* uv, const int* indexes,
        const int vertexCount, const int indexCount) const noexcept {
        SDL_RenderGeometryRaw(sdlRenderer, comonTexture, xy, sizeof(float) * 2, &WHITE, 0,
            uv, sizeof(float) * 2, vertexCount, indexes, indexCount, sizeof(int));
    }
    //
    SDL_Renderer* getRawSDLRenderer() { return sdlRenderer; }
    void setTranslation(const PixelCoord translation) noexcept { this->translation = translation; }
    void setComonTexture(SDL_Texture* texture) { comonTexture = texture; }
    void setRawSDLRenderer(SDL_Renderer* renderer) {
        sdlRenderer = renderer;
        SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
    } 
private:
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;
};
