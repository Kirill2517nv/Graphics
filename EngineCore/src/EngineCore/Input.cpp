#include "EngineCore/Input.hpp"

namespace Engine {
    bool Input::mKeysPressed[static_cast<size_t>(KeyCode::KEY_LAST)] = {};

    bool Input::isKeyPressed(const KeyCode keyCode)
    {
        return mKeysPressed[static_cast<size_t>(keyCode)];
    }

    void Input::pressKey(const KeyCode keyCode)
    {
        mKeysPressed[static_cast<size_t>(keyCode)] = true;
    }

    void Input::releaseKey(const KeyCode keyCode)
    {
        mKeysPressed[static_cast<size_t>(keyCode)] = false;
    }
}
