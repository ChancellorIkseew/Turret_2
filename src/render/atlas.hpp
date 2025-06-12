#pragma once
#include <SDL3/SDL_render.h>
#include <filesystem>
#include <unordered_map>

class Atlas {
    static inline SDL_Texture* comonTexture = nullptr;
    static inline SDL_Renderer* renderer;
public:
    static void clear();
    static void build();
    static void addTexture(const std::filesystem::path& path);
    static void testDraw();
    static SDL_Texture* rawSDL() { return comonTexture; }
    static SDL_FRect at(const std::string& name);
    static void setRenderer(SDL_Renderer* renderer) {
        Atlas::renderer = renderer;
    }
};
