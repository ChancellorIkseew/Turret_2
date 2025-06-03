#pragma once
#include <unordered_map>
#include <SDL3/SDL_events.h>
#include "binding.hpp"

class Input {
	static std::unordered_map<BindName, Binding> bindings;
public:
	static void update(const SDL_Event& event);
	///@brief Check any press/click.
	static bool active(const BindName bindName);
	///@brief Check only short press/click.
	static bool jactive(const BindName bindName);
	///@brief Last symbol entered in any text field.
	/*
	static void updateMouseCoord(sf::RenderWindow& window);
	///@brief Coordinate in SFML window.
	static sf::Vector2i getMouseCoord();
	///@brief Pixel coordinate on map.
	static sf::Vector2f getMouseMapCoord();
	///@brief (none/up/down)
	static t1::MouseWheelScroll getMouseWheelScroll();
	*/
	//static void rebind(const BindName keyName, const SDL_Event& event);
};