#include <window/window.hpp>

class Engine {
	MainWindow mainWindow;
public:
	Engine(const std::string& windowTitle) : mainWindow(windowTitle) { }
	void run();
};