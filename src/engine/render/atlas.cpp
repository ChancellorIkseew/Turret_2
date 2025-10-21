#include "atlas.hpp"
//
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_render.h>
#include <unordered_map>
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
#include "packer/packer.hpp"

namespace fs = std::filesystem;
constexpr Uint32 TRANSPARENT = 0U;
static SDL_Point size;
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
    if (!io::folders::fileExists(path)) {
        logger.error() << "Image file does not exist: " << path;
        return;
    }
    std::string name = path.filename().stem().string();
    if (atlas.contains(name)) {
        logger.warning() << "Texture with name \"" << name << "\" already exists.";
        return;
    }
    SDL_Surface* surface = IMG_Load(path.string().c_str());
    if (!surface) {
        logger.error() << "Texture was not created. File: " << path << " " << SDL_GetError();
        return;
    }
    atlas.emplace(name, SDL_Rect(0, 0, surface->w, surface->h));
    temporarySurfaces.emplace(name, surface);
}

void Atlas::build() {
    size = packer::arrangeRects(atlas);
    SDL_Surface* comonSurface = SDL_CreateSurface(size.x, size.y, SDL_PIXELFORMAT_RGBA8888);
    SDL_FillSurfaceRect(comonSurface, nullptr, TRANSPARENT);
    for (auto& [name, rect] : atlas) {
        SDL_BlitSurface(temporarySurfaces.at(name), nullptr, comonSurface, &rect);
    }
    comonTexture = SDL_CreateTextureFromSurface(renderer, comonSurface);
    SDL_SetTextureScaleMode(comonTexture, SDL_SCALEMODE_NEAREST);
    SDL_DestroySurface(comonSurface);
    clearTemporary();
}

SDL_FRect Atlas::at(const std::string& name) {
    if (!atlas.contains(name)) {
        logger.error() << "Texture was not created yet or does not exist: " << name;
        return SDL_FRect(0.0f, 0.0f, 0.0f, 0.0f);
    }
    SDL_FRect fRect;
    auto& iRect = atlas[name];
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

void Atlas::testDraw() {
    SDL_FRect rect(0, 0, static_cast<float>(size.x), static_cast<float>(size.y));
    SDL_RenderTexture(renderer, Atlas::rawSDL(), nullptr, &rect);
}
