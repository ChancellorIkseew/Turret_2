#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include "engine/coords/pixel_coord.hpp"

class Atlas;
class Renderer;

namespace text {
    void drawString(Renderer& renderer, const std::u32string_view text, const PixelCoord pos, const uint32_t color);
    void setFont(const Atlas& atlas, const std::string& latin, const std::string& cyrilic);
}
