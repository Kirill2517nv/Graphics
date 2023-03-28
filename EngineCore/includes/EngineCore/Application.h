#pragma once

#include <memory>
#include "EngineCore/Event.hpp"
#include "EngineCore/Camera.hpp"
#include "EngineCore/Rendering/OpenGL/Material.hpp"


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
		
		void close() { mbCloseWindow = true; };

		virtual void onUpdate() {};

		virtual void onUiDraw() {};

		virtual void onMouseButtonEvent(const MouseButton button, const double xPos, const double yPos, const bool pressed) {};

		glm::vec2 getCurrentCursorPosition() const;

		float cameraPosition[3] = { 0.f, 0.f, 0.f };
		float cameraRotation[3] = { 0.f, 0.f, 0.f };

		// lightning parameters
		float light_source_pos[3] = { 5.f, 5.f, 2.f };
		float light_source_color[3] = {1.f, 1.f, 1.f};
		float ls_brightness = 10;

		float ambient_factor = 0.2f;
		float diffuse_factor = 0.2f;
		float specular_factor = 2;
		float shininess = 2;

		float camera_fov = 60.f;
		float camera_near_plane = 0.1f;
		float camera_far_plane = 100.f;

		bool perspectiveCamera = true;
		Camera camera{ glm::vec3(-10.f, 0.f, 2.f) }; // starting position of camera
	private:
		void draw();
		std::shared_ptr<class Window> mpWindow;
		EventDispatcher mEventDispatcher;
		bool mbCloseWindow = false;

	};
}