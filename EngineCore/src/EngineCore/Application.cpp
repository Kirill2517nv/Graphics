#include <iostream>
#include "EngineCore/Application.h"
#include "EngineCore/Log.h"
#include "EngineCore/Window.h"


namespace Engine {

	Application::Application() {
		LOG_INFO("Starting application");
	}

	Application::~Application() {
		LOG_INFO("Closing application");
	}

	int Application::start(unsigned int window_width, unsigned int window_height, const char* title)
	{
		mpWindow = std::make_unique<Window>(title, window_width, window_height);
		mpWindow->set_event_callback(
			[](Event& event) {
				LOG_INFO("[EVENT] Change size to {0} x {1}", event.width, event.height);
			}
		);

		while (true) {
			mpWindow->onUpdate();
			onUpdate();
		}

		return 0;
	}
}