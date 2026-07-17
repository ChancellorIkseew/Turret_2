#pragma once
#include <bit>
#include <cmath>
#include <vector>
#include "config.hpp"
#include "glad/glad.h"
#include "texture_rect.hpp"

struct Vertex {
    float x, y;     // position
    float u, v;     // texcoord
    uint32_t color; // ARGB
    float tx, ty;   // local coords in texture rect
};

class RenderGeometry {
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int ebo = 0;

    std::vector<Vertex> vertexAccumulator;

    static constexpr size_t MAX_SPRITES = 10000;
    static constexpr size_t MAX_VERTICES = MAX_SPRITES * 4;
    static constexpr size_t MAX_INDICES = MAX_SPRITES * 6;
public:
    RenderGeometry() {
        constexpr GLuint VERTEX_SLOT_0 = 0;
        constexpr GLint COMPONENTS_VEC2 = 2;
        constexpr GLint BUFFER_OFFSET_START = 0;
        enum Loc : GLuint { position = 0, texcoord = 1, color = 2, local_tex = 3 };

        glCreateVertexArrays(1, &vao);
        glCreateBuffers(1, &vbo);
        glCreateBuffers(1, &ebo);

        glNamedBufferStorage(vbo, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_STORAGE_BIT);
        glNamedBufferStorage(ebo, MAX_INDICES * sizeof(unsigned int), nullptr, GL_DYNAMIC_STORAGE_BIT);

        glVertexArrayVertexBuffer(vao, VERTEX_SLOT_0, vbo, BUFFER_OFFSET_START, sizeof(Vertex));
        glVertexArrayElementBuffer(vao, ebo);
        
        glEnableVertexArrayAttrib(vao, Loc::position);
        glVertexArrayAttribFormat(vao, Loc::position, COMPONENTS_VEC2, GL_FLOAT, GL_FALSE, offsetof(Vertex, x));
        glVertexArrayAttribBinding(vao, Loc::position, VERTEX_SLOT_0);
        //
        glEnableVertexArrayAttrib(vao, Loc::texcoord);
        glVertexArrayAttribFormat(vao, Loc::texcoord, COMPONENTS_VEC2, GL_FLOAT, GL_FALSE, offsetof(Vertex, u));
        glVertexArrayAttribBinding(vao, Loc::texcoord, VERTEX_SLOT_0);
        //
        glEnableVertexArrayAttrib(vao, Loc::color);
        glVertexArrayAttribFormat(vao, Loc::color, GL_BGRA, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex, color));
        glVertexArrayAttribBinding(vao, Loc::color, VERTEX_SLOT_0);
        //
        glEnableVertexArrayAttrib(vao, Loc::local_tex);
        glVertexArrayAttribFormat(vao, Loc::local_tex, COMPONENTS_VEC2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tx));
        glVertexArrayAttribBinding(vao, Loc::local_tex, VERTEX_SLOT_0);

        vertexAccumulator.reserve(MAX_VERTICES);
        std::vector<unsigned int> indices;
        indices.reserve(MAX_INDICES);
        unsigned int offset = 0;
        for (size_t i = 0; i < MAX_SPRITES; ++i) {
            indices.push_back(offset + 0);
            indices.push_back(offset + 1);
            indices.push_back(offset + 2);
            indices.push_back(offset + 3);
            indices.push_back(offset + 0);
            indices.push_back(offset + 2);
            offset += 4;
        }
        glNamedBufferSubData(ebo, 0, indices.size() * sizeof(unsigned int), indices.data());
    }

    ~RenderGeometry() {
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    bool isFull() const noexcept {
        return vertexAccumulator.size() >= MAX_VERTICES;
    }

    void addIrregularQuad(const TextureRect& textureRect,
        const PixelCoord p0, const PixelCoord p1,
        const PixelCoord p2, const PixelCoord p3,
        const uint32_t colorRGBA)
    {
        Vertex quad[4];

        quad[0].x = p0.x; quad[0].y = p0.y;
        quad[1].x = p1.x; quad[1].y = p1.y;
        quad[2].x = p2.x; quad[2].y = p2.y;
        quad[3].x = p3.x; quad[3].y = p3.y;

        float uMax = textureRect.x + textureRect.w;
        float vMax = textureRect.y + textureRect.h;

        quad[0].u = textureRect.x; quad[0].v = textureRect.y;
        quad[1].u = uMax;          quad[1].v = textureRect.y;
        quad[2].u = uMax;          quad[2].v = vMax;
        quad[3].u = textureRect.x; quad[3].v = vMax;

        const uint32_t colorARGB = std::rotr(colorRGBA, 8);
        quad[0].color = colorARGB;
        quad[1].color = colorARGB;
        quad[2].color = colorARGB;
        quad[3].color = colorARGB;

        quad[0].tx = p0.x; quad[0].ty = p0.y; // World positions,
        quad[1].tx = p1.x; quad[1].ty = p1.y; // not local coords
        quad[2].tx = p2.x; quad[2].ty = p2.y; // in texture rect.
        quad[3].tx = p3.x; quad[3].ty = p3.y;

        vertexAccumulator.insert(vertexAccumulator.end(), quad, quad + 4);
    }

    void addQuad(const TextureRect& textureRect,
        const PixelCoord position, const PixelCoord size,
        const PixelCoord origin, const float angleRad,
        const uint32_t colorRGBA)
    {
        float localX[4] = { 0.f - origin.x, size.x - origin.x, size.x - origin.x, 0.f - origin.x };
        float localY[4] = { 0.f - origin.y, 0.f - origin.y, size.y - origin.y, size.y - origin.y };

        Vertex quad[4];

        if (std::abs(angleRad) > 0.00001f) {
            float cosA = std::cos(angleRad);
            float sinA = std::sin(angleRad);

            for (int i = 0; i < 4; ++i) {
                quad[i].x = position.x + (localX[i] * cosA - localY[i] * sinA);
                quad[i].y = position.y + (localX[i] * sinA + localY[i] * cosA);
            }
        }
        else {
            for (int i = 0; i < 4; ++i) {
                quad[i].x = position.x + localX[i];
                quad[i].y = position.y + localY[i];
            }
        }

        float uMax = textureRect.x + textureRect.w;
        float vMax = textureRect.y + textureRect.h;

        quad[0].u = textureRect.x; quad[0].v = textureRect.y;
        quad[1].u = uMax;          quad[1].v = textureRect.y;
        quad[2].u = uMax;          quad[2].v = vMax;
        quad[3].u = textureRect.x; quad[3].v = vMax;

        const uint32_t colorARGB = std::rotr(colorRGBA, 8);
        quad[0].color = colorARGB;
        quad[1].color = colorARGB;
        quad[2].color = colorARGB;
        quad[3].color = colorARGB;

        quad[0].tx = 0.f;    quad[0].ty = 0.f;
        quad[1].tx = size.x; quad[1].ty = 0.f;
        quad[2].tx = size.x; quad[2].ty =  size.y;
        quad[3].tx = 0.f;    quad[3].ty =  size.y;

        vertexAccumulator.insert(vertexAccumulator.end(), quad, quad + 4);
    }

    void flush() {
        if (vertexAccumulator.empty())
            return;
        constexpr GLintptr BUFFER_OFFSET_START = 0;
        glNamedBufferSubData(vbo, BUFFER_OFFSET_START, vertexAccumulator.size() * sizeof(Vertex), vertexAccumulator.data());
        glBindVertexArray(vao);
        //
        const size_t spriteCount = vertexAccumulator.size() / 4;
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(spriteCount * 6), GL_UNSIGNED_INT, nullptr);
        vertexAccumulator.clear();
    }
private:
    t1_disable_copy_and_move(RenderGeometry)
};
