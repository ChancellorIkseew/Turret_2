#include "packer.hpp"
//
#define STB_RECT_PACK_IMPLEMENTATION
#include <STB/stb_rect.h>
#include <iostream>

void packer::arrangeRects(std::unordered_map<std::string, SDL_Rect>& atlas) {
    std::vector<stbrp_rect> rects(atlas.size());
    int i = 0;
    for (const auto& [name, rect] : atlas) {
        rects[i].id = i;
        rects[i].w = rect.w;
        rects[i].h = rect.h;
        ++i;
    }
    //
    std::vector<stbrp_node> nodes(atlas.size());
    stbrp_context context;
    for (i = 0; i < 5; ++i) {
        stbrp_init_target(&context, 128, 64, nodes.data(), static_cast<int>(atlas.size()));
        int result = stbrp_pack_rects(&context, rects.data(), static_cast<int>(nodes.size()));
        if (result != 0)
            break;
    }
    //
    i = 0;
    for (auto& [name, rect] : atlas) {
        rect.x = rects[i].x;
        rect.y = rects[i].y;
        std::cout << name << ": " << rect.x << " " << rect.y << '\n';
        ++i;
    }
}
