#pragma once
#include <string>
#include <unordered_map>
#include "engine/coords/pixel_coord.hpp"
#include "engine/render/texture_rect.hpp"

namespace packer {
    PixelCoord arrangeRects(std::unordered_map<std::string, TextureRect>& atlas);
}
