
#include "SDL3/SDL_opengl.h"
#include <cmath>
#pragma comment(lib, "OpenGL32.lib")

#define PI 3.14159265358979323846f
#define DEG2RAD (PI/180.0f)
#define RAD2DEG (180.0f/PI)


using Uint32 = uint32_t;

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct Vector2 {
    float x, y;
    Vector2(const float x, const float y) : x(x), y(y) { }
    Vector2() = default;
};

struct Texture {
    Uint32 id, x, y, w, h;
    Texture(const Uint32 id, const Uint32 x, const Uint32 y, const Uint32 w, const Uint32 h) :
        id(id), x(x), y(y), w(w), h(h) { }
};

struct Rect {
    float w, h, x, y;
    Rect(const Uint32 x, const Uint32 y, const Uint32 w, const Uint32 h) :
        x(static_cast<float>(x)), y(static_cast<float>(y)),
        w(static_cast<float>(w)), h(static_cast<float>(h)) { }
};


static void drawTexturePro(Texture texture, Rect source, Rect dest, Vector2 origin, float rotation, Color color) {
    // Check if texture is valid
    if (texture.id > 0)
    {
        float width = (float)texture.w;
        float height = (float)texture.h;

        bool flipX = false;

        if (source.w < 0) 
            flipX = true; source.w *= -1;
        if (source.h < 0) 
            source.y -= source.h;

        if (dest.w < 0) 
            dest.w *= -1;
        if (dest.h < 0) 
            dest.h *= -1;

        Vector2 topLeft;
        Vector2 topRight;
        Vector2 bottomLeft;
        Vector2 bottomRight;

        // Only calculate rotation if needed
        if (rotation == 0.0f) {
            float x = dest.x - origin.x;
            float y = dest.y - origin.y;
            topLeft = Vector2(x, y);
            topRight = Vector2(x + dest.w, y);
            bottomLeft = Vector2(x, y + dest.h);
            bottomRight = Vector2(x + dest.w, y + dest.h);
        }
        else {
            float sinRotation = sinf(rotation * DEG2RAD);
            float cosRotation = cosf(rotation * DEG2RAD);
            float x = dest.x;
            float y = dest.y;
            float dx = -origin.x;
            float dy = -origin.y;

            topLeft.x = x + dx * cosRotation - dy * sinRotation;
            topLeft.y = y + dx * sinRotation + dy * cosRotation;

            topRight.x = x + (dx + dest.w) * cosRotation - dy * sinRotation;
            topRight.y = y + (dx + dest.w) * sinRotation + dy * cosRotation;

            bottomLeft.x = x + dx * cosRotation - (dy + dest.h) * sinRotation;
            bottomLeft.y = y + dx * sinRotation + (dy + dest.h) * cosRotation;

            bottomRight.x = x + (dx + dest.w) * cosRotation - (dy + dest.h) * sinRotation;
            bottomRight.y = y + (dx + dest.w) * sinRotation + (dy + dest.h) * cosRotation;
        }

        glBindTexture(GL_TEXTURE_2D, texture.id);
        glBegin(GL_TRIANGLE_FAN);

        glColor4ub(color.r, color.g, color.b, color.a);
        glNormal3f(0.0f, 0.0f, 1.0f);                          // Normal vector pointing towards viewer

        // Top-left corner for texture and quad
        if (flipX) glTexCoord2f((source.x + source.w) / width, source.y / height);
        else glTexCoord2f(source.x / width, source.y / height);
        glVertex2f(topLeft.x, topLeft.y);

        // Bottom-left corner for texture and quad
        if (flipX) glTexCoord2f((source.x + source.w) / width, (source.y + source.h) / height);
        else glTexCoord2f(source.x / width, (source.y + source.h) / height);
        glVertex2f(bottomLeft.x, bottomLeft.y);

        // Bottom-right corner for texture and quad
        if (flipX) glTexCoord2f(source.x / width, (source.y + source.h) / height);
        else glTexCoord2f((source.x + source.w) / width, (source.y + source.h) / height);
        glVertex2f(bottomRight.x, bottomRight.y);

        // Top-right corner for texture and quad
        if (flipX) glTexCoord2f(source.x / width, source.y / height);
        else glTexCoord2f((source.x + source.w) / width, source.y / height);
        glVertex2f(topRight.x, topRight.y);

        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0); // reset

    }
}


