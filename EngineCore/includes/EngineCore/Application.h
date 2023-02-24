#pragma once

#include <memory>

namespace Engine {
	class Application {
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;


		virtual int start(unsigned int window_width, unsigned int window_height, const char* title);

		virtual void onUpdate() {};
	private:
		std::unique_ptr<class Window> mpWindow;
		EventDispatcher mEventDispatcher;
		bool mbCloseWindow;

	};
}