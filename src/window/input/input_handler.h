#pragma once
#ifndef T1_SYSTEM_INPUT_HANDLER_H
#define T1_SYSTEM_INPUT_HANDLER_H

#include <unordered_map>
#include "t1_input.h"


class InputHandler
{
private:
	static std::unordered_map<t1::BindName, t1::Binding> bindings;

public:
	static void updateInput(const SDL_Event& event);
	///@brief Check any press/click.
	static bool active(const t1::BindName bindName);
	///@brief Check only short press/click.
	static bool jactive(const t1::BindName bindName);
	///@brief Last symbol entered in any text field.
	static char32_t getLastSymbolEntered();
	/*
	static void updateMouseCoord(sf::RenderWindow& window);
	///@brief Coordinate in SFML window.
	static sf::Vector2i getMouseCoord();
	///@brief Pixel coordinate on map.
	static sf::Vector2f getMouseMapCoord();
	///@brief (none/up/down)
	static t1::MouseWheelScroll getMouseWheelScroll();
	*/
	static void rebind(const t1::BindName keyName, const SDL_Event& event);
};

#endif // T1_SYSTEM_INPUT_HANDLER_H
