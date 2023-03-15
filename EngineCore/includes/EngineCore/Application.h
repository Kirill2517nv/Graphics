#pragma once

#include <memory>
#include "EngineCore/Event.hpp"
#include "EngineCore/Camera.hpp"
#include <vector>

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

		virtual void onUiDraw() {};

		virtual void onMouseButtonEvent(const MouseButton button, const double xPos, const double yPos, const bool pressed) {};

		glm::vec2 getCurrentCursorPosition() const;

		float cameraPosition[3] = { 0.f, 0.f, 1.f };
		float cameraRotation[3] = { 0.f, 0.f, 0.f };
		bool perspectiveCamera = true;
		Camera camera{ glm::vec3(-5.f, 0.f, 0.f) };
	private:
		std::unique_ptr<class Window> mpWindow;
		EventDispatcher mEventDispatcher;
		bool mbCloseWindow = false;


	};
}