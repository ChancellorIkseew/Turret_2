#pragma once
#ifdef 0
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

class Sprite {
    SDL_Renderer* renderer_;
    SDL_Texture* texture_ = nullptr;
    int width_ = 0;
    int height_ = 0;
public:
    Sprite(SDL_Renderer* renderer, const std::string& filePath) : renderer_(renderer) {
        texture_ = IMG_LoadTexture(renderer_, filePath.c_str());
        if (texture_ == nullptr) {
            std::cerr << "SDL_LoadTexture Error: " << SDL_GetError() << std::endl;
            // Обработка ошибки (например, выброс исключения)
        }

        SDL_QueryTexture(texture_, nullptr, nullptr, &width_, &height_); // Получение размеров текстуры
    }

    ~Sprite() {
        SDL_DestroyTexture(texture_);
    }

    // Методы доступа (getters)
    SDL_Texture* getTexture() const {
        return texture_;
    }

    int GetWidth() const {
        return width_;
    }

    int GetHeight() const {
        return height_;
    }

    // Метод отрисовки спрайта
    void Draw(float x, float y) {
        SDL_FRect destRect;
        destRect.x = x;
        destRect.y = y;
        destRect.w = static_cast<float>(width_);
        destRect.h = static_cast<float>(height_);

        SDL_RenderCopyF(renderer_, texture_, nullptr, &destRect);
    }
    void Draw(float x, float y, float w, float h)
    {
        SDL_FRect destRect;
        destRect.x = x;
        destRect.y = y;
        destRect.w = w;
        destRect.h = h;

        SDL_RenderCopyF(renderer_, texture_, nullptr, &destRect);
    } 
};

#endif