#include "packer.hpp"
//
#define STB_RECT_PACK_IMPLEMENTATION
#include <STB/stb_rect.h>
#include <bit>
#include <vector>
#include "engine/debug/logger.hpp"

constexpr int MAX_PACK_ATTEMPTS = 5;
static debug::Logger logger("atlas_packer");

static PixelCoord calculateSize(const int square, const int maxWidth, const int maxHeight) {
    int w = maxWidth, h = maxHeight;
    if (maxWidth >= maxHeight)
        h = std::max(square / maxWidth, maxHeight);
    else
        w = std::max(square / maxHeight, maxWidth);
    w = std::bit_ceil(uint32_t(w));
    h = std::bit_ceil(uint32_t(h));
    logger.info("1st packing attempt size: x{} y{}", w, h);
    return PixelCoord(w, h);
}

PixelCoord packer::arrangeRects(std::unordered_map<std::string, TextureRect>& atlas) {
    std::vector<stbrp_rect> rects(atlas.size());
    int square = 0, maxWidth = 0, maxHeight = 0;
    int i = 0;
    for (const auto& [name, rect] : atlas) {
        stbrp_rect stbRect{ .id = i, .w = stbrp_coord(rect.w + 1.f), .h = stbrp_coord(rect.h + 1.f) };
        if (stbRect.w > maxWidth)
            maxWidth = stbRect.w;
        if (stbRect.h > maxHeight)
            maxHeight = stbRect.h;
        square += stbRect.w * stbRect.h;
        rects[i] = stbRect;
        ++i;
    }
    PixelCoord size = calculateSize(square, maxWidth, maxHeight);
    //
    std::vector<stbrp_node> nodes(atlas.size());
    stbrp_context context;
    for (i = 1;; ++i) {
        stbrp_init_target(&context, int(size.x), int(size.y), nodes.data(), int(nodes.size()));
        int result = stbrp_pack_rects(&context, rects.data(), int(rects.size()));
        if (result != 0)
            break;
        if (i >= MAX_PACK_ATTEMPTS) {
            logger.error("Packing failed. Attempts: {}. Final attemtp size: x{} y{}", i, size.x, size.y);
            break;
        }
        if (size.x >= size.y)
            size.y *= 2.f;
        else
            size.x *= 2.f;
    }
    //
    i = 0;
    for (auto& [name, rect] : atlas) {
        rect.x = float(rects[i].x);
        rect.y = float(rects[i].y);
        logger.debug("Texture placed: \"{}\" position: x{} y{}", name, rect.x, rect.y);
        ++i;
    }
    logger.info("Atlas packed. Size: x{} y{}", size.x, size.y);
    return size;
 }
