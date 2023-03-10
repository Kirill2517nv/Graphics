#include <iostream>


#include "EngineCore/Application.h"
#include "EngineCore/Log.h"
#include "EngineCore/Window.h"
#include "EngineCore/Event.hpp"

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

		mEventDispatcher.addEventListener<EventMouseMoved>(
			[](EventMouseMoved& event) {
				LOG_INFO("[MouseMoved] Mouse moved to {0} x {1}", event.x, event.y);
			}
		);

		mEventDispatcher.addEventListener<EventWindowResize>(
			[](EventWindowResize& event) {
				LOG_INFO("[WindowResized] Changed size to {0} x {1}", event.width, event.height);
			}
		);

		mEventDispatcher.addEventListener<EventWindowClose>(
			[&](EventWindowClose& event) {
				LOG_INFO("[WindowClose]");
				mbCloseWindow = true;
			}
		);


		mpWindow->set_event_callback(
			[&](BaseEvent& event) {
				mEventDispatcher.dispatch(event);
			}
		);

		while (!mbCloseWindow) {
			mpWindow->onUpdate();
			onUpdate();
		}
		mpWindow = nullptr;

		return 0;
	}
}