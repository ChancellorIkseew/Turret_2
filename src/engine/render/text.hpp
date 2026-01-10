#pragma once
#include <string>
#include "engine/coords/pixel_coord.hpp"

class Atlas;
class Renderer;

namespace text {
    void drawString(const Renderer& renderer, const std::u32string& text, const PixelCoord pos);
    void setFont(const Atlas& atlas, const std::string& latin, const std::string& cyrilic, const std::string& custom);
    void loadCustomSymbols();
    char32_t getCustomSymbol(const std::string& symbolName);
}
