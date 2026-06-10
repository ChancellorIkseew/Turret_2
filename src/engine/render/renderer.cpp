#include "renderer.hpp"
//
#include <SDL3/SDL.h>
#include <stdexcept>
#include "glad/glad.h"
#include "framebuffer.hpp"
#include "shader_program.hpp"
#include "texture.hpp"
#include "vertex_buffer.hpp"

Renderer::Renderer(SDL_Window* sdlWindow) {
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
}

Renderer::~Renderer() {
    if (glContext)
        SDL_GL_DestroyContext(glContext);
}

void Renderer::setShaderProgram(const ShaderProgram& shaderProgram) {
    const Pipeline pipeline = shaderProgram.getPipeline();

    if (pipeline.useLightmap != targetLightmap) {
        flush();
        targetLightmap = pipeline.useLightmap;
        if (targetLightmap) {
            lightmapFBO->bind();
            glClear(GL_COLOR_BUFFER_BIT);
        }
        else {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTextureUnit(1, lightmapFBO->getTextureID());
        }
        currentTextureID = 0;
    }

    if (shaderProgram.getID() != currentShaderProgramID) {
        flush();
        currentShaderProgramID = shaderProgram.getID();
        glUseProgram(currentShaderProgramID);
    }

    glEnable(GL_BLEND);
    glBlendFunc(pipeline.srcBlend, pipeline.dstBlend);
}

void Renderer::setView(float scale, const PixelCoord translation, const PixelCoord windowSize) {
    GLfloat view[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    view[0] = (2.0f * scale) / windowSize.x;
    view[5] = (-2.0f * scale) / windowSize.y;
    view[10] = -1.0f;

    view[12] = -1.0f + (-2.0f * translation.x / windowSize.x) * scale;
    view[13] = 1.0f - (-2.0f * translation.y / windowSize.y) * scale;

    glProgramUniformMatrix4fv(currentShaderProgramID, 0, 1, GL_FALSE, view);
}

void Renderer::present(SDL_Window* sdlWindow) {
    flush();
    SDL_GL_SwapWindow(sdlWindow);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::resize(const int x, const int y) {
    glViewport(0, 0, x, y);
    if (lightmapFBO)
        lightmapFBO->resize(x, y);
}

void Renderer::flush() {
    if (batchGeometry->isEmpty())
        return;
    batchGeometry->upload();
    batchGeometry->bind();
    size_t spriteCount = batchGeometry->getVerticesCount() / 4;
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(spriteCount * 6), GL_UNSIGNED_INT, nullptr);
    batchGeometry->clear();
}

void Renderer::draw(const Texture2D& texture2D, const TextureRect& textureRect,
    const PixelCoord position, const PixelCoord size,
    const PixelCoord origin, const double angleRad, const uint32_t color)
{
    if (texture2D.getID() != currentTextureID || batchGeometry->isFull()) {
        flush();
        currentTextureID = texture2D.getID();
        constexpr unsigned int SLOT = 0;
        glBindTextureUnit(SLOT, currentTextureID);
    }

    batchGeometry->addQuad(textureRect, position, size, origin, angleRad, color);
}

void Renderer::draw(const TextureRect& textureRect,
    const PixelCoord position, const PixelCoord size,
    const PixelCoord origin, const double angleRad, const uint32_t color)
{
    if (atlasTextureID != currentTextureID || batchGeometry->isFull()) {
        flush();
        currentTextureID = atlasTextureID;
        constexpr unsigned int SLOT = 0;
        glBindTextureUnit(SLOT, currentTextureID);
    }

    batchGeometry->addQuad(textureRect, position, size, origin, angleRad, color);
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
    atlasTexture = std::make_unique<Texture2D>(converted->w, converted->h, static_cast<TextureData>(converted->pixels));
    atlasTextureID = atlasTexture->getID();
    SDL_DestroySurface(converted);
}
