#pragma once
#include "glad/glad.h"

class Texture2D {
    unsigned int textureID = 0;
public:
    Texture2D(int width, int height, const unsigned char* data) {
        // 1. Создаем объект текстуры типа GL_TEXTURE_2D
        glCreateTextures(GL_TEXTURE_2D, 1, &textureID);

        // 2. Выделяем неизменяемую память на GPU (Immutable Storage)
        // 1 — это количество уровней мипмаппинга (нам пока нужен только один)
        glTextureStorage2D(textureID, 1, GL_RGBA8, width, height);

        // 3. Загружаем пиксели в выделенную память
        glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // 4. Настраиваем фильтрацию (прорисовка пикселей при масштабировании)
        // GL_NEAREST идеально подходит для пиксель-арта и тайлов, так как сохраняет четкие границы
        glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Настройка повторения текстуры (если выходим за границы UV 0.0 - 1.0)
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    ~Texture2D() {
        glDeleteTextures(1, &textureID);
    }

    unsigned int getID() const { return textureID; }
private:
    //Texture2D(const Texture2D&) = delete;
    //Texture2D& operator=(const Texture2D&) = delete; 
};
