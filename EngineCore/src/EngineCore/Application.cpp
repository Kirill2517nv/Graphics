#include <iostream>


#include "EngineCore/Application.h"
#include "EngineCore/Log.h"
#include "EngineCore/Window.h"
#include "EngineCore/Event.hpp"
#include "EngineCore/Input.hpp"

#include "EngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "EngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "EngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "EngineCore/Rendering/OpenGL/IndexBuffer.h"
#include "EngineCore/Rendering/OpenGL/Texture2D.hpp"
#include "EngineCore/Camera.hpp"
#include "EngineCore/Rendering/OpenGL/RendererOpenGL.hpp"
#include "EngineCore/Modules/UIModule.hpp"

#include <imgui/imgui.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>

namespace Engine {

	GLfloat positionsColorsCoords[] = {
		0.0f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   2.f, -1.f,
		0.0f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,  -1.f, -1.f,
		0.0f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,   2.f,  2.f,
		0.0f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  -1.f,  2.f,
		0.0f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   10.f, 0.f,
		0.0f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   0.f,  0.f,
		0.0f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,   10.f, 10.f,
		0.0f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.f,  10.f
	};

	GLuint indices[] = {
		0, 1, 2, 3, 2, 1
	};
	//------------------------- TEST FUNCTIONS FOR SIMPLE TEXTURE GENERATION --------------//
	void generate_circle(unsigned char* data,
		const unsigned int width,
		const unsigned int height,
		const unsigned int center_x,
		const unsigned int center_y,
		const unsigned int radius,
		const unsigned char color_r,
		const unsigned char color_g,
		const unsigned char color_b)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				if ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) < radius * radius)
				{
					data[3 * (x + width * y) + 0] = color_r;
					data[3 * (x + width * y) + 1] = color_g;
					data[3 * (x + width * y) + 2] = color_b;
				}
			}
		}
	}
	void generate_smile_texture(unsigned char* data,
		const unsigned int width,
		const unsigned int height)
	{
		// background
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				data[3 * (x + width * y) + 0] = 200;
				data[3 * (x + width * y) + 1] = 191;
				data[3 * (x + width * y) + 2] = 231;
			}
		}
		// face
		generate_circle(data, width, height, width * 0.5, height * 0.5, width * 0.4, 255, 255, 0);
		// smile
		generate_circle(data, width, height, width * 0.5, height * 0.4, width * 0.2, 0, 0, 0);
		generate_circle(data, width, height, width * 0.5, height * 0.45, width * 0.2, 255, 255, 0);
		// eyes
		generate_circle(data, width, height, width * 0.35, height * 0.6, width * 0.07, 255, 0, 255);
		generate_circle(data, width, height, width * 0.65, height * 0.6, width * 0.07, 0, 0, 255);
	}
	void generate_quads_texture(unsigned char* data,
		const unsigned int width,
		const unsigned int height)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				if ((x < width / 2 && y < height / 2) || x >= width / 2 && y >= height / 2)
				{
					data[3 * (x + width * y) + 0] = 0;
					data[3 * (x + width * y) + 1] = 0;
					data[3 * (x + width * y) + 2] = 0;
				}
				else
				{
					data[3 * (x + width * y) + 0] = 255;
					data[3 * (x + width * y) + 1] = 255;
					data[3 * (x + width * y) + 2] = 255;
				}
			}
		}
	}
	//---------------------------------------//
	const char* vertexShader =
		R"(#version 460
           layout(location = 0) in vec3 vertex_position;
           layout(location = 1) in vec3 vertex_color;
           layout(location = 2) in vec2 texture_coord;
           uniform mat4 model_matrix;
           uniform mat4 view_projection_matrix;
           uniform int current_frame; 
           out vec3 color;
           out vec2 tex_coord_smile;
           out vec2 tex_coord_quads;
           void main() {
              color = vertex_color;
              tex_coord_smile = texture_coord;
              tex_coord_quads = texture_coord + vec2(current_frame / 1000.f, current_frame / 1000.f);
              gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
           }
        )";

	const char* fragmentShader =
		R"(#version 460
           in vec3 color;
           in vec2 tex_coord_smile;
           in vec2 tex_coord_quads;
           layout (binding = 0) uniform sampler2D InTexture_Smile;
           layout (binding = 1) uniform sampler2D InTexture_Quads;
           out vec4 frag_color;
           void main() {
              //frag_color = vec4(color, 1.0);
              frag_color = texture(InTexture_Smile, tex_coord_smile) * texture(InTexture_Quads, tex_coord_quads);
           }
        )";

	std::unique_ptr<ShaderProgram> pShaderProgram;
	std::unique_ptr<VertexBuffer> pPositionsColorsVbo;
	std::unique_ptr<IndexBuffer> pIndexBuffer;
	std::unique_ptr<Texture2D> p_texture_smile;
	std::unique_ptr<Texture2D> p_texture_quads;
	std::unique_ptr<VertexArray> pVao;
	float scale[3] = { 1.f, 1.f, 1.f };
	float rotate = 0.f;
	float translate[3] = { 0.f, 0.f, 0.f };
	float mBackgroundColor[4] = { 0.33f, 0.33f, 0.33f, 0.f };

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
				//LOG_INFO("[MouseMoved] Mouse moved to {0} x {1}", event.x, event.y);
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

		mEventDispatcher.addEventListener<EventKeyPressed>(
			[&](EventKeyPressed& event) {
				
				if (event.keycode <= KeyCode::KEY_Z) {
					if (event.repeated) {
						LOG_INFO("[KEY PRESSED] {0}, repeated", (char)event.keycode);
					}
					else {
						LOG_INFO("[KEY PRESSED] {0}", static_cast<char>(event.keycode));
					}
				}
				Input::pressKey(event.keycode);
			}
		);

		mEventDispatcher.addEventListener<EventKeyReleased>(
			[&](EventKeyReleased& event) {
				
				if (event.keycode <= KeyCode::KEY_Z) {
					LOG_INFO("[KEY RELEASED] {0}", static_cast<char>(event.keycode));
				}
				Input::releaseKey(event.keycode);
			}
		);
		// mouse event
		mEventDispatcher.addEventListener<EventMouseButtonPressed>(
			[&](EventMouseButtonPressed& event) {
				LOG_INFO("[MOUSE BUTTON PRESSED: {0} AT {1}x{2}]", static_cast<int>(event.mouseButton), event.xPos, event.yPos);
				Input::pressMouseButton(event.mouseButton);
				onMouseButtonEvent(event.mouseButton, event.xPos, event.yPos, true);
			}
		);

		mEventDispatcher.addEventListener<EventMouseButtonReleased>(
			[&](EventMouseButtonReleased& event) {
				LOG_INFO("[MOUSE BUTTON RELEASED: {0} AT {1}x{2}]", static_cast<int>(event.mouseButton), event.xPos, event.yPos);
				Input::releaseMouseButton(event.mouseButton);
				onMouseButtonEvent(event.mouseButton, event.xPos, event.yPos, false);
			}
		);

		mpWindow->set_event_callback(
			[&](BaseEvent& event) {
				mEventDispatcher.dispatch(event);
			}
		);

		const unsigned int width = 1000;
		const unsigned int height = 1000;
		const unsigned int channels = 3;
		auto* data = new unsigned char[width * height * channels];
		generate_smile_texture(data, width, height);
		
		p_texture_smile = std::make_unique<Texture2D>(data, width, height);
		p_texture_smile->bind(0);
		
		generate_quads_texture(data, width, height);
		p_texture_quads = std::make_unique<Texture2D>(data, width, height);
		p_texture_quads->bind(1);

		delete[] data;
		//---------------------------------------//
		pShaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
		if (!pShaderProgram->isCompiled())
		{
			return false;
		}

		BufferLayout bufferLayout1vec3
		{
			ShaderDataType::Float3
		};


		BufferLayout bufferLayoutVec3Vec3Vec2
		{
			ShaderDataType::Float3,
			ShaderDataType::Float3,
			ShaderDataType::Float2
		};

		pVao = std::make_unique<VertexArray>();
		pPositionsColorsVbo = std::make_unique<VertexBuffer>(positionsColorsCoords, sizeof(positionsColorsCoords), bufferLayoutVec3Vec3Vec2);
		pIndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

		pVao->addVertexBuffer(*pPositionsColorsVbo);
		pVao->setIndexBuffer(*pIndexBuffer);
		//---------------------------------------//
		static int current_frame = 0;

		while (!mbCloseWindow) {
			RendererOpenGL::setClearColor(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2], mBackgroundColor[3]);
			RendererOpenGL::clear();

			pShaderProgram->bind();

			glm::mat4 scaleMatrix(scale[0], 0, 0, 0,
				0, scale[1], 0, 0,
				0, 0, scale[2], 0,
				0, 0, 0, 1);

			float rotateInRadians = glm::radians(rotate);
			glm::mat4 rotateMatrix(cos(rotateInRadians), sin(rotateInRadians), 0, 0,
				-sin(rotateInRadians), cos(rotateInRadians), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);

			glm::mat4 translateMatrix(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate[0], translate[1], translate[2], 1);

			glm::mat4 modelMatrix = translateMatrix * rotateMatrix * scaleMatrix;
			pShaderProgram->setMatrix4("model_matrix", modelMatrix);
			pShaderProgram->setInt("current_frame", current_frame++);

			camera.setProjectionMode(perspectiveCamera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
			pShaderProgram->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
			RendererOpenGL::draw(*pVao);


			//---------------------------------------//
			UIModule::onUiDrawBegin();
			bool show = true;
			UIModule::ShowExampleAppDockSpace(&show);
			ImGui::ShowDemoWindow();
			ImGui::Begin("Background Color Window");
			ImGui::ColorEdit4("Background Color", mBackgroundColor);
			ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
			ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
			ImGui::SliderFloat3("translate", translate, -1.f, 1.f);
			ImGui::SliderFloat3("camera position", cameraPosition, -10.f, 10.f);
			ImGui::SliderFloat3("camera rotation", cameraRotation, 0, 360.f);
			ImGui::Checkbox("Perspective camera", &perspectiveCamera);
			ImGui::End();
			//---------------------------------------//

			onUiDraw();

			UIModule::onUiDrawEnd();

			mpWindow->onUpdate();
			onUpdate();
		}
		mpWindow = nullptr;

		return 0;
	}

	glm::vec2 Application::getCurrentCursorPosition() const {
		return mpWindow->getCurrentCursorPosition();
	}
}