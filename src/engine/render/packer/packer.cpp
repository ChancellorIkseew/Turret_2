#include "packer.hpp"
//
#define STB_RECT_PACK_IMPLEMENTATION
#include <STB/stb_rect.h>
#include <vector>
#include "engine/coords/transforms.hpp"
#include "engine/debug/logger.hpp"

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
    logger.info() << "1st packing attempt size: " << w << " " << h;
    return SDL_Point(w, h);
}

SDL_Point packer::arrangeRects(std::unordered_map<std::string, SDL_Rect>& atlas) {
    std::vector<stbrp_rect> rects(atlas.size());
    int square = 0, maxWidth = 0, maxHeight = 0;
    int i = 0;
    for (const auto& [name, rect] : atlas) {
        rects[i].id = i;
        rects[i].w = rect.w + 1;
        rects[i].h = rect.h + 1;
        if (rect.w + 1 > maxWidth)
            maxWidth = rect.w + 1;
        if (rect.h + 1 > maxHeight)
            maxHeight = rect.h + 1;
        square += (rect.w + 1) * (rect.h + 1);
        ++i;
    }
    SDL_Point size = calculateSize(square, maxWidth, maxHeight);
    //
    std::vector<stbrp_node> nodes(atlas.size());
    stbrp_context context;
    for (i = 1;; ++i) {
        stbrp_init_target(&context, size.x, size.y, nodes.data(), static_cast<int>(nodes.size()));
        int result = stbrp_pack_rects(&context, rects.data(), static_cast<int>(rects.size()));
        if (result != 0)
            break;
        if (i >= MAX_PACK_ATTEMPTS) {
            logger.error() << "Packing failed. Attempts: " << i << ". Final attemtp size: " << size.x << " " << size.y;
            break;
        }
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
        logger.debug() << "Texture placed: \"" << name << "\" position: " << rect.x << " " << rect.y;
        ++i;
    }
    logger.info() << "Atlas packed. Size: " << size.x << " " << size.y;
    return size;
 }
