#include "EngineCore/Input.hpp"

namespace Engine {
    bool Input::mKeysPressed[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};
    bool Input::mMouseButtonsPressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1] = {};
    
    bool Input::isKeyPressed(const KeyCode keyCode) {
        return mKeysPressed[static_cast<size_t>(keyCode)];
    }

    void Input::pressKey(const KeyCode keyCode) {
        mKeysPressed[static_cast<size_t>(keyCode)] = true;
    }

    void Input::releaseKey(const KeyCode keyCode) {
        mKeysPressed[static_cast<size_t>(keyCode)] = false;
    }

    bool Input::isMouseButtonPressed(const MouseButton button) {
        return mMouseButtonsPressed[static_cast<size_t>(button)];
    }

    void Input::pressMouseButton(const MouseButton button) {
        mMouseButtonsPressed[static_cast<size_t>(button)] = true;
    }

    void Input::releaseMouseButton(const MouseButton button) {
        mMouseButtonsPressed[static_cast<size_t>(button)] = false;
    }
}
