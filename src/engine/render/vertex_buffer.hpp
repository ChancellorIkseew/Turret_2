#pragma once
#include <cmath>
#include <vector>
#include "glad/glad.h"
#include "texture_rect.hpp"

struct Vertex {
    float x, y;     // position
    float u, v;     // texcoord
    uint32_t color; // RGBA
};

class RenderGeometry {
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int ebo = 0;

    std::vector<Vertex> vertexAccumulator;
    size_t maxVerticesCount = 0;

public:
    RenderGeometry() {
        size_t maxSprites = 10000;
        maxVerticesCount = maxSprites * 4;
        size_t maxIndices = maxSprites * 6;

        vertexAccumulator.reserve(maxVerticesCount);

        glCreateVertexArrays(1, &vao);
        glCreateBuffers(1, &vbo);
        glCreateBuffers(1, &ebo);

        glNamedBufferStorage(vbo, maxVerticesCount * sizeof(Vertex), nullptr, GL_DYNAMIC_STORAGE_BIT);
        glNamedBufferStorage(ebo, maxIndices * sizeof(unsigned int), nullptr, GL_DYNAMIC_STORAGE_BIT);

        glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
        // Связываем EBO с VAO (для индексных вызовов)
        glVertexArrayElementBuffer(vao, ebo);

        // Описываем атрибуты (0 - Позиция, 1 - UV, 2 - Цвет)
        glEnableVertexArrayAttrib(vao, 0);
        glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, x));
        glVertexArrayAttribBinding(vao, 0, 0);

        glEnableVertexArrayAttrib(vao, 1);
        glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, u));
        glVertexArrayAttribBinding(vao, 1, 0);

        // === НАШЕ ИЗМЕНЕНИЕ: ДОБАВЛЯЕМ АТРИБУТ ЦВЕТА ===
        glEnableVertexArrayAttrib(vao, 2);
        // 4 компонента GL_UNSIGNED_BYTE (по 1 байту на R, G, B, A). 
        // GL_TRUE нормализует 0..255 в 0.0f..1.0f в шейдере автоматически.
        glVertexArrayAttribFormat(vao, 2, GL_BGRA, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex, color));
        glVertexArrayAttribBinding(vao, 2, 0);

        // ЗАПЕКАЕМ ИНДЕКСЫ ОДИН РАЗ ПРИ СТАРТЕ
        std::vector<unsigned int> indices;
        indices.reserve(maxIndices);
        unsigned int offset = 0;
        for (size_t i = 0; i < maxSprites; ++i) {
            indices.push_back(offset + 0);
            indices.push_back(offset + 1);
            indices.push_back(offset + 2);
            indices.push_back(offset + 2);
            indices.push_back(offset + 3);
            indices.push_back(offset + 0);
            offset += 4;
        }
        glNamedBufferSubData(ebo, 0, indices.size() * sizeof(unsigned int), indices.data());
    }

    ~RenderGeometry() {
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    // Проверка на заполненность буфера
    bool isFull() const noexcept {
        return vertexAccumulator.size() >= maxVerticesCount;
    }

    bool isEmpty() const noexcept {
        return vertexAccumulator.empty();
    }

    size_t getVerticesCount() const noexcept {
        return vertexAccumulator.size();
    }

    // Очистка аккумулятора на CPU
    void clear() noexcept {
        vertexAccumulator.clear();
    }

    // Метод генерации геометрии для одного спрайта
    void addQuad(const TextureRect& textureRect,
        const PixelCoord position, const PixelCoord size,
        const PixelCoord origin, const double angleRad,
        uint32_t color = 0xFFFFFFFF) // <-- Добавили аргумент по умолчанию
    {
        float localX[4] = { 0.0f - origin.x, size.x - origin.x, size.x - origin.x, 0.0f - origin.x };
        float localY[4] = { 0.0f - origin.y, 0.0f - origin.y, size.y - origin.y, size.y - origin.y };

        Vertex quad[4];

        if (std::abs(angleRad) > 0.00001) {
            float cosA = std::cos(static_cast<float>(angleRad));
            float sinA = std::sin(static_cast<float>(angleRad));

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

        // === ЗАПИСЫВАЕМ ЦВЕТ В КАЖДУЮ ВЕРШИНУ ===
        quad[0].color = color;
        quad[1].color = color;
        quad[2].color = color;
        quad[3].color = color;

        vertexAccumulator.insert(vertexAccumulator.end(), quad, quad + 4);
    }

    // Загрузка накопленного в GPU
    void upload() {
        glNamedBufferSubData(vbo, 0, vertexAccumulator.size() * sizeof(Vertex), vertexAccumulator.data());
    }

    void bind() const { glBindVertexArray(vao); }
};
