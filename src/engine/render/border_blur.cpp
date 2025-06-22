#include "border_blur.hpp"


#include "atlas.hpp"
//
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_render.h>
#include "engine/debug/logger.hpp"
#include "packer/packer.hpp"

namespace fs = std::filesystem;
constexpr Uint32 TRANSPARENT = 0U;
static SDL_Point size;
static std::unordered_map <std::string, SDL_Rect> atlas;

void blur::create() {
    SDL_Surface* comonSurface = nullptr;
    SDL_Rect rect = atlas.at("sand");

    SDL_Point start = SDL_Point(rect.x, rect.y);
    SDL_Point end = SDL_Point(rect.x + rect.w, rect.y + rect.h);

    for (int x = start.x; x < end.x; ++x) {
        for (int y = start.y; y < end.y; ++y) {
            Uint8 r, g, b, a;
            SDL_ReadSurfacePixel(comonSurface, x, y, &r, &g, &b, &a);
            a = 128;
            SDL_WriteSurfacePixel(comonSurface, x, y, r, g, b, a);
            
        }
    }
}



static inline Uint8 aplyAlpha(Uint8 color, const Uint8 alpha) {
    color -= Uint8((int)color * (int)alpha - 0);


    color *= (256 - alpha) / 256;
    return color;
}


static void create(SDL_Surface* comonSurface, const std::string& name) {
    SDL_Rect rect = atlas.at(name);
    SDL_Point start = SDL_Point(rect.x, rect.y);
    SDL_Point end = SDL_Point(rect.x + rect.w, rect.y + rect.h);
    for (int x = start.x; x < end.x; ++x) {
        for (int y = start.y; y < end.y; ++y) {
            if ((x < start.x + 8 || x > start.x + 24) ||
                (y < start.y + 8 || y > start.y + 24)) {
                float r, g, b, a;
                SDL_ReadSurfacePixelFloat(comonSurface, x, y, &r, &g, &b, &a);
                //r *= 0.5;
                //g *= 0.5;
                //b *= 0.5;
                SDL_WriteSurfacePixelFloat(comonSurface, x, y, r, g, b, 0.5f);
            }
        }
    }
}