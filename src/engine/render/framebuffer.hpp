#pragma once
#include <stdexcept>
#include "config.hpp"
#include "glad/glad.h"

class LightmapFramebuffer {
    unsigned int fbo = 0;
    unsigned int textureID = 0;
    int currentWidth = 0;
    int currentHeight = 0;

public:
    LightmapFramebuffer() {
        init(1280, 720);
    }

    ~LightmapFramebuffer() {
        cleanup();
    }

    void init(int width, int height) {
        currentWidth = width;
        currentHeight = height;

        constexpr GLsizei FRAMEBUFFERS_COUNT = 1;
        glCreateFramebuffers(FRAMEBUFFERS_COUNT, &fbo);
        //
        constexpr GLsizei TEXTURES_COUNT = 1;
        glCreateTextures(GL_TEXTURE_2D, TEXTURES_COUNT, &textureID);
        // 
        constexpr GLsizei MIPMAP_LEVELS_COUNT = 1;
        glTextureStorage2D(textureID, MIPMAP_LEVELS_COUNT, GL_RGB8, width, height);
        //
        glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //
        constexpr GLint MIPMAP_LEVEL = 0;
        glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, textureID, MIPMAP_LEVEL);

        if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Framebuffer is not complete!");
        }
    }

    void resize(int width, int height) {
        if (width == currentWidth && height == currentHeight)
            return;
        cleanup();
        init(width, height);
    }

    void bindForRender() const noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    unsigned int getTextureID() const noexcept {
        return textureID;
    }

private:
    void cleanup() {
        if (fbo) glDeleteFramebuffers(1, &fbo);
        if (textureID) glDeleteTextures(1, &textureID);
        fbo = 0;
        textureID = 0;
    }

    t1_disable_copy_and_move(LightmapFramebuffer)
};
