#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include "config.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "surface.hpp"
#include "texture_rect.hpp"

class Renderer;

class Atlas {
    PixelCoord size;
    std::unordered_map <std::string, TextureRect> atlas;
    std::unordered_map <std::string, Surface> temporarySurfaces;
public:
    Atlas() = default;
    ~Atlas() { clear(); }
    //
    void clear();
    void build(Renderer& renderer);
    void addTexture(const std::filesystem::path& path);
    TextureRect at(const std::string& name) const noexcept;
    PixelCoord getSize() const noexcept { return size; }
private:
    t1_disable_copy_and_move(Atlas)
};
