#include "EngineCore/Window.h"
#include "EngineCore/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {
	Window::Window(std::string title, const unsigned int width, const unsigned int height) {
		//: mTitle(std::move(title)),
	}

	Window::~Window() {

	}

	void Window::onUpdate() {

	}

	int Window::init() {
		return 0;
	}

	void Window::shutdown() {

	}
}