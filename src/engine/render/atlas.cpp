#include "atlas.hpp"
//
#include <SDL3/SDL_render.h>
#include <unordered_map>
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
#include "packer/packer.hpp"
#include "renderer.hpp"

namespace fs = std::filesystem;
constexpr Uint32 TRANSPARENT = 0U;
static debug::Logger logger("texture_atlas");

static void clearTemporary(std::unordered_map <std::string, SDL_Surface*>& temporarySurfaces) {
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
    SDL_Surface* surface = SDL_LoadPNG(path.string().c_str());
    if (!surface) {
        logger.error() << "Texture was not created. File: " << path << " " << SDL_GetError();
        return;
    }
    atlas.emplace(name, SDL_Rect(0, 0, surface->w, surface->h));
    temporarySurfaces.emplace(name, surface);
}

void Atlas::build(Renderer& renderer) {
    size = packer::arrangeRects(atlas);
    SDL_Surface* comonSurface = SDL_CreateSurface(size.x, size.y, SDL_PIXELFORMAT_RGBA8888);
    SDL_FillSurfaceRect(comonSurface, nullptr, TRANSPARENT);
    for (auto& [name, rect] : atlas) {
        SDL_BlitSurface(temporarySurfaces.at(name), nullptr, comonSurface, &rect);
    }
    renderer.createCommonTexture(comonSurface);
    SDL_DestroySurface(comonSurface);
    clearTemporary(temporarySurfaces);
}

Texture Atlas::at(const std::string& name) const noexcept {
    if (!atlas.contains(name)) {
        logger.error() << "Texture was not created yet or does not exist: " << name;
        return NULL_TEXTURE;
    }
    SDL_FRect fRect;
    auto& iRect = atlas.at(name);
    fRect.x = static_cast<float>(iRect.x);
    fRect.y = static_cast<float>(iRect.y);
    fRect.w = static_cast<float>(iRect.w);
    fRect.h = static_cast<float>(iRect.h);
    return Texture(fRect);
}

void Atlas::clear() {
    atlas.clear();
    clearTemporary(temporarySurfaces);
}

Texture Atlas::getComonTexture() const noexcept {
    return Texture(SDL_FRect(0.0f, 0.0f, static_cast<float>(size.x), static_cast<float>(size.y)));
}
