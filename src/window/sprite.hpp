#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "window.hpp"

class Sprite {
    SDL_Renderer* renderer;
    SDL_Texture* texture = nullptr;
    SDL_FRect rect;
    int width_ = 0;
    int height_ = 0;
public:
    Sprite(SDL_Renderer* renderer, const std::string& filePath) : renderer(renderer) {
        SDL_Surface* healthbar_sur = IMG_Load("res/images/icon.bmp");
        texture = SDL_CreateTextureFromSurface(renderer, healthbar_sur);
        SDL_DestroySurface(healthbar_sur); // Free surface after creating texture

        rect = SDL_FRect(100, 100, 200, 200);
    }

    ~Sprite() {
        SDL_DestroyTexture(texture);
    }

    SDL_Texture* getTexture() const {
        return texture;
    }

    int GetWidth() const {
        return width_;
    }

    int GetHeight() const {
        return height_;
    }

    void draw() {
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }

    void Draw(float x, float y, float w, float h)
    {

    } 
};