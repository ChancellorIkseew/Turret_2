#include "renderer.hpp"
//
#include <SDL3/SDL_video.h>
#include <stdexcept>
#include "glad/glad.h"
#include "framebuffer.hpp"
#include "render_geometry.hpp"
#include "shader_program.hpp"

Renderer::Renderer(SDL_Window* sdlWindow, const PixelCoord viewportSize) : viewportSize(viewportSize) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    glContext = SDL_GL_CreateContext(sdlWindow);
    if (!glContext) {
        const std::string error = SDL_GetError();
        throw std::runtime_error("SDL_GL_CreateContext Error: " + error);
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_GL_DestroyContext(glContext);
        throw std::runtime_error("GLAD initialization failed");
    }

    batchGeometry = std::make_unique<RenderGeometry>();
    lightmapFBO = std::make_unique<LightmapFramebuffer>();
    setYSincMode(YSincMode::adaptive);
}

Renderer::~Renderer() {
    if (glContext)
        SDL_GL_DestroyContext(glContext);
}

void Renderer::setShaderProgram(const ShaderProgram& shaderProgram) {
    const Pipeline pipeline = shaderProgram.getPipeline();

    if (pipeline.useLightmap != targetLightmap) {
        batchGeometry->flush();
        targetLightmap = pipeline.useLightmap;
        if (targetLightmap) {
            lightmapFBO->bindForRender();
            glClear(GL_COLOR_BUFFER_BIT);
        }
        else {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTextureUnit(1, lightmapFBO->getTextureID());
        }
        currentTextureID = 0;
    }

    if (shaderProgram.getID() != currentShaderProgramID) {
        batchGeometry->flush();
        currentShaderProgramID = shaderProgram.getID();
        glUseProgram(currentShaderProgramID);
    }

    glEnable(GL_BLEND);
    glBlendFunc(pipeline.srcBlend, pipeline.dstBlend);
}

void Renderer::setView(float scale, const PixelCoord translation) {
    flush();
    GLfloat view[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    view[0] = (2.0f * scale) / viewportSize.x;
    view[5] = (-2.0f * scale) / viewportSize.y;
    view[10] = -1.0f;

    view[12] = -1.0f + (-2.0f * translation.x / viewportSize.x) * scale;
    view[13] = 1.0f - (-2.0f * translation.y / viewportSize.y) * scale;

    glProgramUniformMatrix4fv(currentShaderProgramID, 0, 1, GL_FALSE, view);
}

void Renderer::clear() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::present(SDL_Window* sdlWindow) {
    batchGeometry->flush();
    SDL_GL_SwapWindow(sdlWindow);
}

void Renderer::resize(const int x, const int y) {
    viewportSize = PixelCoord(x, y);
    glViewport(0, 0, x, y);
    if (lightmapFBO)
        lightmapFBO->resize(x, y);
}

void Renderer::setYSincMode(const YSincMode mode) {
    SDL_GL_SetSwapInterval(mode);
    int newMode = 0;
    SDL_GL_GetSwapInterval(&newMode);
    ySincMode = YSincMode(newMode);
}

void Renderer::draw(const Texture2D& texture2D, const TextureRect& textureRect,
    const PixelCoord position, const PixelCoord size,
    const PixelCoord origin, const float angleRad, const uint32_t color)
{
    if (texture2D.getID() != currentTextureID || batchGeometry->isFull()) {
        batchGeometry->flush();
        currentTextureID = texture2D.getID();
        constexpr unsigned int SLOT = 0;
        glBindTextureUnit(SLOT, currentTextureID);
    }

    batchGeometry->addQuad(textureRect, position, size, origin, angleRad, color);
}

void Renderer::draw(const TextureRect& textureRect,
    const PixelCoord position, const PixelCoord size,
    const PixelCoord origin, const float angleRad, const uint32_t color)
{
    draw(atlasTexture.value(), textureRect, position, size, origin, angleRad, color);
}

static Texture2D convert(SDL_Surface* rawSurface) {
    if (!rawSurface)
        throw std::runtime_error(SDL_GetError());

    SDL_Surface* optimizedRaw = SDL_ConvertSurface(rawSurface, SDL_PIXELFORMAT_RGBA32);
    if (!optimizedRaw) {
        throw std::runtime_error("Failed to convert surface format: " + std::string(SDL_GetError()));
    }

    return Texture2D(optimizedRaw->w, optimizedRaw->h,
        static_cast<const unsigned char*>(optimizedRaw->pixels));
}

void Renderer::createAtlasTexture(SDL_Surface* sdlSurface) {
    if (!sdlSurface)
        return;
    SDL_Surface* converted = SDL_ConvertSurface(sdlSurface, SDL_PIXELFORMAT_RGBA32);
    if (!converted)
        return;
    using TextureData = const unsigned char*;
    atlasTexture.emplace(converted->w, converted->h, static_cast<TextureData>(converted->pixels));
    SDL_DestroySurface(converted);
}

std::string Renderer::takeScreenshot() const {
    const int width = int(viewportSize.x);
    const int height = int(viewportSize.y);
    const size_t bufferSize = static_cast<size_t>(width * height * 4);
    std::string pixels;
    pixels.resize(bufferSize);

    constexpr int SCREEN_BUFFER = 0;
    glBindFramebuffer(GL_READ_FRAMEBUFFER, SCREEN_BUFFER);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    return pixels;
}
