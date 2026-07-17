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
    Surface comonSurface(SDL_CreateSurface(size.x, size.y, SDL_PIXELFORMAT_RGBA8888));
    SDL_FillSurfaceRect(comonSurface.raw(), nullptr, TRANSPARENT);
    for (auto& [name, rect] : atlas) {
        SDL_BlitSurface(temporarySurfaces.at(name).raw(), nullptr, comonSurface.raw(), &rect);
    }
    renderer.createAtlasTexture(comonSurface.raw());
    temporarySurfaces.clear();
    renderer.setWhiteRect(Atlas::at("white_rect"));
}

TextureRect Atlas::at(const std::string& name) const noexcept {
    if (!atlas.contains(name)) {
        logger.error() << "Texture was not created yet or does not exist: " << name;
        return NULL_TEXTURE_RECT;
    }
    TextureRect fRect;
    auto& iRect = atlas.at(name);
    fRect.x = static_cast<float>(iRect.x) / size.x;
    fRect.y = static_cast<float>(iRect.y) / size.y;
    fRect.w = static_cast<float>(iRect.w) / size.x;
    fRect.h = static_cast<float>(iRect.h) / size.y;
    return fRect;
}

void Atlas::clear() {
    atlas.clear();
    temporarySurfaces.clear();
}
