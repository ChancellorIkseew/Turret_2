#pragma once
#include <stdexcept>
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

        // 1. Создаем объект фреймбуфера через DSA
        glCreateFramebuffers(1, &fbo);

        // 2. Создаем текстуру, куда будет записываться свет кадра
        glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
        // Выделяем память (1 уровень мипмаппинга, формат RGB8 достаточно для света)
        glTextureStorage2D(textureID, 1, GL_RGB8, width, height);

        // Настраиваем фильтрацию для карты света. 
        // Здесь GL_LINEAR идеален — он автоматически размоет края наших источников света!
        glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // 3. Привязываем текстуру к нашему фреймбуферу как «цветной выход №0»
        glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, textureID, 0);

        // Check на валидность фреймбуфера
        if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Framebuffer is not complete!");
        }
    }

    void resize(int width, int height) {
        // Если размер окна изменился (например, ресайз), нужно пересоздать текстуру под новый размер
        if (width == currentWidth && height == currentHeight) return;
        cleanup();
        init(width, height);
    }

    // Перенаправить вывод рендера в эту текстуру
    void bind() const noexcept {
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

    LightmapFramebuffer(const LightmapFramebuffer&) = delete;
    LightmapFramebuffer& operator=(const LightmapFramebuffer&) = delete;
};
