#pragma once
#include <SDL3/SDL.h>
#include <filesystem>

class MainWindow;

class Texture {
    static inline SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
public:
    Texture(const std::filesystem::path& path);
    ~Texture() {
        SDL_DestroyTexture(texture);
    }
    //
    SDL_Texture* rawSDL() const { return texture; }
private:
    friend MainWindow;
    static void setRenderer(SDL_Renderer* renderer) {
        Texture::renderer = renderer;
    }
};
