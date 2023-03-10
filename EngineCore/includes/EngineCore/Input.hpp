#pragma once

#include "Keys.hpp"

namespace Engine {

	class Input {
	public:
		static bool isKeyPressed(const KeyCode keyCode);
		static void pressKey(const KeyCode keyCode);
		static void releaseKey(const KeyCode keyCode);

	private:
		static bool mKeysPressed[static_cast<size_t>(KeyCode::KEY_LAST)];
	};
}