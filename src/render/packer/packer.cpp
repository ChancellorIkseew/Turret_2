#include "packer.hpp"
//
#define STB_RECT_PACK_IMPLEMENTATION
#include <STB/stb_rect.h>
#include "coords/transforms.hpp"
#include "debug/logger.hpp"

constexpr int MAX_PACK_ATTEMPTS = 5;
static debug::Logger logger("atlas_packer");

static SDL_Point calculateSize(const int square, const int maxWidth, const int maxHeight) {
    int w = maxWidth, h = maxHeight;
    if (maxWidth >= maxHeight)
        h = std::max(square / maxWidth, maxHeight);
    else
        w = std::max(square / maxHeight, maxWidth);
    w = t1::ceilUpPow2(w);
    h = t1::ceilUpPow2(h);
    logger.info() << "1st render attempt size: " << w << " " << h;
    return SDL_Point(w, h);
}

void packer::arrangeRects(std::unordered_map<std::string, SDL_Rect>& atlas) {
    std::vector<stbrp_rect> rects(atlas.size());
    int square = 0, maxWidth = 0, maxHeight = 0;
    int i = 0;
    for (const auto& [name, rect] : atlas) {
        rects[i].id = i;
        rects[i].w = rect.w;
        rects[i].h = rect.h;
        if (rect.w > maxWidth)
            maxWidth = rect.w;
        if (rect.h > maxHeight)
            maxHeight = rect.h;
        square += rect.w * rect.h;
        ++i;
    }
    SDL_Point size = calculateSize(square, maxWidth, maxHeight);
    //
    std::vector<stbrp_node> nodes(atlas.size());
    stbrp_context context;
    for (i = 0; i < MAX_PACK_ATTEMPTS; ++i) {
        stbrp_init_target(&context, size.x, size.y, nodes.data(), static_cast<int>(atlas.size()));
        int result = stbrp_pack_rects(&context, rects.data(), static_cast<int>(nodes.size()));
        if (result != 0)
            break;
        if (size.x >= size.y)
            size.y *= 2;
        else
            size.x *= 2;
    }
    //
    i = 0;
    for (auto& [name, rect] : atlas) {
        rect.x = rects[i].x;
        rect.y = rects[i].y;
        logger.info() << "Texture rendered: \"" << name << "\" position: " << rect.x << " " << rect.y;
        ++i;
    }
    logger.info() << "Atlas created. Size: " << size.x << " " << size.y;
 }
