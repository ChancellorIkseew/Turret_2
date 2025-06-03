#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <filesystem>
class MainWindow;

class Texture {
	static inline SDL_Renderer* renderer;
	SDL_Texture* texture = nullptr;
public:
	Texture(std::filesystem::path path) {
		texture = IMG_LoadTexture(renderer, path.string().c_str());
	}
	~Texture() {
		SDL_DestroyTexture(texture);
	}
	//
	SDL_Texture* rawSDL() { return texture; }
private:
	friend MainWindow;
	static void setRenderer(SDL_Renderer* renderer) {
		Texture::renderer = renderer;
	}
};