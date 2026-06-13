#pragma once
#include <memory>
#include <optional>
#include <string>
#include "engine/coords/pixel_coord.hpp"
#include "texture_2d.hpp"

struct SDL_Surface;
struct SDL_Window;
struct SDL_GLContextState;
typedef struct SDL_GLContextState* SDL_GLContext;

class LightmapFramebuffer;
class RenderGeometry;
class ShaderProgram;
struct TextureRect;

class Renderer {
    SDL_GLContext glContext = nullptr;
    unsigned int currentShaderProgramID = 0xFF'FF'FF'FF;
    unsigned int currentTextureID = 0xFF'FF'FF'FF;
    std::unique_ptr<RenderGeometry> batchGeometry;
    std::unique_ptr<LightmapFramebuffer> lightmapFBO;
    std::optional<Texture2D> atlasTexture;
    PixelCoord viewportSize;
    bool targetLightmap = false;
public:
    Renderer(SDL_Window* sdlWindow, const PixelCoord viewportSize);
    ~Renderer();
    //
    void setView(float scale, const PixelCoord translation);
    void setShaderProgram(const ShaderProgram& shaderProgram);
    void resize(const int x, const int y);
    //
    void present(SDL_Window* sdlWindow);
    //
    void draw(const Texture2D& texture2D, const TextureRect& textureRect,
        const PixelCoord position, const PixelCoord size,
        const PixelCoord origin, const float angleRad, const uint32_t color);

    void draw(const TextureRect& textureRect,
        const PixelCoord position, const PixelCoord size,
        const PixelCoord origin = {0.f, 0.f}, const float angleRad = 0.f, const uint32_t color = 0xFF'FF'FF'FF);

    void createAtlasTexture(SDL_Surface* sdlSurface);
    //
    std::string takeScreenshot() const;
private:
    void flush();
    t1_disable_copy_and_move(Renderer)
};
