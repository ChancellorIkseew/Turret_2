#include "input.hpp"
//
#include <atomic>

constexpr char32_t NON_USABLE_SYMBOL = static_cast<char32_t>(0);
static std::atomic<char32_t> symbolJustEntered = NON_USABLE_SYMBOL;
static std::atomic<SDL_Point> mouseCoord = SDL_Point(0, 0);
static std::atomic<SDL_FPoint> mouseMapCoord = SDL_FPoint(0.0f, 0.0f);
static std::atomic<MouseWheelScroll> mouseWheelScroll = MouseWheelScroll::none;

void Input::update(const SDL_Event& event) {
	//if (event.type == SDL_EVENT_MOUSE_WHEEL)
		//mouseWheelScroll.store(static_cast<t1::MouseWheelScroll>(event.wheel.y), std::memory_order_relaxed);

	InputType inputType = InputType::keyboard;
	int code = -1;
	bool pressed = false;

	if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
		code = event.key.scancode;
		pressed = event.key.down;
	} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
		pressed = event.button.down;
		code = event.button.button;
		inputType = InputType::mouse;
	} else {
		return;
	}

	for (auto& [bindName, binding] : bindings) {
		if (inputType != binding.inputType || code != binding.code)
			continue;
		binding.justTriggered = !binding.active;
		binding.active = pressed;
	}
}


bool Input::active(const BindName bindName) {
	return bindings.contains(bindName) && bindings.at(bindName).active;
}

bool Input::jactive(const BindName bindName) {
	const auto& found = bindings.find(bindName);
	if (found == bindings.end() || !found->second.justTriggered)
		return false;
	found->second.justTriggered = false;
	return true;
}