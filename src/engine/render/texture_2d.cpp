#include "texture_2d.hpp"
//
#include "glad/glad.h"

Texture2D::Texture2D(int width, int height, const unsigned char* data) {
    constexpr GLsizei TEXTURES_COUNT = 1;
    glCreateTextures(GL_TEXTURE_2D, TEXTURES_COUNT, &textureID);
    //
    constexpr GLsizei MIPMAPING_LEVELS_COUNT = 1;
    glTextureStorage2D(textureID, MIPMAPING_LEVELS_COUNT, GL_RGBA8, width, height);
    //
    constexpr GLint MIPMAPING_LEVEL = 0;
    constexpr GLint OFFSET_X = 0;
    constexpr GLint OFFSET_Y = 0;
    glTextureSubImage2D(textureID, MIPMAPING_LEVEL, OFFSET_X, OFFSET_Y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //
    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &textureID);
}
