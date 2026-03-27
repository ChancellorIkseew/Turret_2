#pragma once
#include <string>
#include <string_view>
#include "engine/coords/pixel_coord.hpp"

class Atlas;
class Renderer;

namespace text {
    void drawString(const Renderer& renderer, const std::u32string_view text, const PixelCoord pos);
    void setFont(const Atlas& atlas, const std::string& latin, const std::string& cyrilic);
}
