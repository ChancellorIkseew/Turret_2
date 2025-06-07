#include "texture.hpp"
//
#include <SDL3_image/SDL_image.h>
#include "debug/logger.hpp"

namespace fs = std::filesystem;
static debug::Logger logger("textures");

Texture::Texture(const fs::path& path) {
    if (!fs::exists(path) || !fs::is_regular_file(path))
        logger.error() << "Image file does not exist: " << path;
    texture = IMG_LoadTexture(renderer, path.string().c_str());
    if (!texture)
        logger.error() << "Texture was not created. File: " << path << " " << SDL_GetError();
}
