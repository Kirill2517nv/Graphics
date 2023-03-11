#pragma once

#include "Keys.hpp"

namespace Engine {

	class Input {
	public:
		static bool isKeyPressed(const KeyCode keyCode);
		static void pressKey(const KeyCode keyCode);
		static void releaseKey(const KeyCode keyCode);

		static bool isMouseButtonPressed(const MouseButton button);
		static void pressMouseButton(const MouseButton button);
		static void releaseMouseButton(const MouseButton button);

	private:
		static bool mKeysPressed[];
		static bool mMouseButtonsPressed[];
	};
}