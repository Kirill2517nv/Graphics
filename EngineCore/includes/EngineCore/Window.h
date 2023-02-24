#pragma once
#include <string>
#include "EngineCore/Events.hpp"
#include <functional>
struct GLFWwindow;

namespace Engine {

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		Window(std::string title, const unsigned int width, const unsigned int height);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void onUpdate();
		unsigned int get_width() const { return mData.width; };
		unsigned int get_height() const { return mData.height; };

		void set_event_callback(const EventCallbackFn& callback) {
			mData.eventCallbackFn = callback;
		}

	private:
		struct WindowData {
			std::string title;
			unsigned int width;
			unsigned int height;
			EventCallbackFn eventCallbackFn;
		};
		int init();
		void shutdown();

		GLFWwindow* mpWindow = nullptr;
		WindowData mData;
	};
}