#include "atlas.hpp"
//
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_render.h>
#include "debug/logger.hpp"
#include "packer/packer.hpp"

namespace fs = std::filesystem;
constexpr Uint32 TRANSPARENT = 0U;
static std::unordered_map <std::string, SDL_Rect> atlas;
static std::unordered_map <std::string, SDL_Surface*> temporarySurfaces;
static debug::Logger logger("texture_atlas");

static void clearTemporary() {
    for (const auto& [_, surface] : temporarySurfaces) {
        SDL_DestroySurface(surface);
    }
    temporarySurfaces.clear();
}

void Atlas::addTexture(const fs::path& path) {
    if (!fs::exists(path) || !fs::is_regular_file(path)) {
        logger.error() << "Image file does not exist: " << path;
        return;
    }
    SDL_Surface* surface = IMG_Load(path.string().c_str());
    if (!surface) {
        logger.error() << "Texture was not created. File: " << path << " " << SDL_GetError();
        return;
    }
    std::string name = path.filename().stem().string();
    atlas.try_emplace(name, SDL_Rect(0, 0, surface->w, surface->h));
    temporarySurfaces.try_emplace(name, surface);
}

void Atlas::build() {
    packer::arrangeRects(atlas);
    SDL_Surface* comonSurface = SDL_CreateSurface(256, 256, SDL_PIXELFORMAT_RGBA8888);
    SDL_FillSurfaceRect(comonSurface, nullptr, TRANSPARENT);
    for (auto& [name, rect] : atlas) {
        SDL_BlitSurface(temporarySurfaces.at(name), nullptr, comonSurface, &rect);
    }
    comonTexture = SDL_CreateTextureFromSurface(renderer, comonSurface);
    SDL_DestroySurface(comonSurface);
    clearTemporary();
}

SDL_FRect Atlas::at(const std::string& name) {
    if (!atlas.contains(name)) {
        logger.error() << "Texture was not created yet or does not exist: " << name;
        return SDL_FRect(0.0f, 0.0f, 0.0f, 0.0f);
    }
    SDL_FRect fRect;
    auto& iRect = atlas.at(name);
    fRect.x = static_cast<float>(iRect.x);
    fRect.y = static_cast<float>(iRect.y);
    fRect.w = static_cast<float>(iRect.w);
    fRect.h = static_cast<float>(iRect.h);
    return fRect;
}

void Atlas::clear() {
    atlas.clear();
    clearTemporary();
    SDL_DestroyTexture(comonTexture);
}
