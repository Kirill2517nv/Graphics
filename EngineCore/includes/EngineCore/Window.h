#pragma once
#include <string>
class GLFWWindow;

namespace Engine {
	class Window {
	public:
		Window(std::string title, const unsigned int width, const unsigned int height);
		virtual ~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void onUpdate();
		unsigned int get_width() const { return mWidth; };
		unsigned int get_height() const { return mHeight; };

	private:
		int init();
		void shutdown();

		GLFWWindow* pWindow;
		std::string mTitle;
		unsigned int mWidth;
		unsigned int mHeight;
	};
}