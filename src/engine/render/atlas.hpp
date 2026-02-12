#pragma once
#include <SDL3/SDL_surface.h>
#include <filesystem>
#include <string>
#include <unordered_map>
#include "config.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "texture.hpp"

class Renderer;

class Atlas {
    SDL_Point size = SDL_Point{ 0, 0 };
    std::unordered_map <std::string, SDL_Rect> atlas;
    std::unordered_map <std::string, SDL_Surface*> temporarySurfaces;
public:
    Atlas() = default;
    ~Atlas() { clear(); }
    //
    void clear();
    void build(Renderer& renderer);
    void addTexture(const std::filesystem::path& path);
    Texture at(const std::string& name) const noexcept;
    Texture getComonTexture() const noexcept;
    PixelCoord getSize() const noexcept { return PixelCoord(size.x, size.y); }
private:
    t1_disable_copy_and_move(Atlas)
};
