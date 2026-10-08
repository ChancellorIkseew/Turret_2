#include "atlas.hpp"
//
#include <SDL3/SDL_render.h>
#include <unordered_map>
#include "engine/debug/logger.hpp"
#include "engine/io/io.hpp"
#include "packer/packer.hpp"
#include "renderer.hpp"

namespace fs = std::filesystem;
constexpr Uint32 TRANSPARENT = 0U;
static debug::Logger logger("texture_atlas");

void Atlas::addTexture(const fs::path& path) {
    std::string name = path.filename().stem().string();
    if (atlas.contains(name)) {
        logger.warning("Texture already exists: \"{}\".", name);
        return;
    }
    std::string blob = io::readFile(path, io::Log::only_error);
    SDL_IOStream* stream = SDL_IOFromConstMem(blob.data(), blob.size());
    Surface surface(SDL_LoadPNG_IO(stream, true));
    if (!surface.raw()) {
        logger.error("Texture was not created. File: {} Error: {}", path.string(), SDL_GetError());
        return;
    }
    atlas.emplace(name, TextureRect(0.f, 0.f, float(surface.raw()->w), float(surface.raw()->h)));
    temporarySurfaces.emplace(name, std::move(surface));
}

void Atlas::build(Renderer& renderer) {
    size = packer::arrangeRects(atlas);

    Surface comonSurface(SDL_CreateSurface(int(size.x), int(size.y), SDL_PIXELFORMAT_RGBA8888));
    SDL_FillSurfaceRect(comonSurface.raw(), nullptr, TRANSPARENT);
    for (auto& [name, rect] : atlas) {
        const SDL_Rect sdlRect{ .x = int(rect.x), .y = int(rect.y), .w = int(rect.w), .h = int(rect.h), };
        SDL_BlitSurface(temporarySurfaces.at(name).raw(), nullptr, comonSurface.raw(), &sdlRect);
        rect.x /= size.x;
        rect.y /= size.y;
        rect.w /= size.x;
        rect.h /= size.y;
    }
    renderer.createAtlasTexture(comonSurface);
    temporarySurfaces.clear();
    renderer.setWhiteRect(Atlas::at("white_rect"));
}

TextureRect Atlas::at(const std::string& name) const noexcept {
    if (atlas.contains(name))
        return atlas.at(name);
    logger.error("Texture was not created yet or does not exist: \"{}\".", name);
    return NULL_TEXTURE_RECT;
}

void Atlas::clear() {
    atlas.clear();
    temporarySurfaces.clear();
}
