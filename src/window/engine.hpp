
#include "window.hpp"
#include "sprite.hpp"

class Engine {
	int FPS = 60, TPS = 60;
	MainWindow mainWindow;

public:
	Engine() : mainWindow("str") {
		
	}

	void setFPS(const int FPS) {

	}
	void setTPS(const int TPS) {

	}


	void draw() {
		Uint32 delta = 0;
		Sprite sprite("str");

		while (true) {
			Uint32 frameStart = SDL_GetTicks();

			mainWindow.clear();
			sprite.draw();
			mainWindow.render();

			Uint32 frameTime = SDL_GetTicks() - frameStart;

			//if (frameDelay > frameTime)
				//SDL_Delay(frameDelay - frameTime);


		}
	}
};