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
#include "EngineCore/Camera.hpp"
#include "EngineCore/Rendering/OpenGL/RendererOpenGL.hpp"
#include "EngineCore/Modules/UIModule.hpp"

#include <imgui/imgui.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>
#define PI 3.14159265

namespace Engine {
	

	GLfloat positionsColors2[] = {
		0.0f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
		0.0f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
		0.0f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
		0.0f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2, 3, 2, 1
	};

	const char* vertexShader =
		R"(#version 460
           layout(location = 0) in vec3 vertexPosition;
           layout(location = 1) in vec3 vertexColor;
           uniform mat4 modelMatrix;
           uniform mat4 viewProjectionMatrix;
           out vec3 color;
           void main() {
              color = vertexColor;
              gl_Position = viewProjectionMatrix * modelMatrix * vec4(vertexPosition, 1.0);
           }
        )";

	const char* fragmentShader =
		R"(#version 460
           in vec3 color;
           out vec4 fragColor;
           void main() {
              fragColor = vec4(color, 1.0);
           }
        )";

	std::unique_ptr<ShaderProgram> pShaderProgram;
	std::unique_ptr<VertexBuffer> pPositionsColorsVbo, pPositionsColorsVbo1;
	std::unique_ptr<IndexBuffer> pIndexBuffer, pIndexBuffer1;
	std::unique_ptr<VertexArray> pVao, pVao1;
	float scale[3] = { 1.f, 1.f, 1.f };
	float rotate = 0.f;
	float translate[3] = { 0.f, 0.f, 0.f };
	float mBackgroundColor[4] = { 0.33f, 0.33f, 0.33f, 0.f };

	Application::Application() {
		LOG_INFO("Starting application");
		float x, y, z, xy;                              // vertex position


		float sectorStep = 2 * PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				mVertices.push_back(x);
				mVertices.push_back(y);
				mVertices.push_back(z);
				if (j <= sectorCount/2) {
					mVertices.push_back(1.0);
					mVertices.push_back(0.0);
					mVertices.push_back(0.0);
				}
				else {
					mVertices.push_back(0.0);
					mVertices.push_back(1.0);
					mVertices.push_back(0.0);
				}

			}
		}
		int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					mIndices.push_back(k1);
					mIndices.push_back(k2);
					mIndices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					mIndices.push_back(k1 + 1);
					mIndices.push_back(k2);
					mIndices.push_back(k2 + 1);
				}
			}
		}


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


		BufferLayout bufferLayout2vec3
		{
			ShaderDataType::Float3,
			ShaderDataType::Float3
		};

	/*	pVao = std::make_unique<VertexArray>();
		pPositionsColorsVbo = std::make_unique<VertexBuffer>(positionsColors2, sizeof(positionsColors2), bufferLayout2vec3);
		pIndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

		pVao->addVertexBuffer(*pPositionsColorsVbo);
		pVao->setIndexBuffer(*pIndexBuffer);*/

		pVao1 = std::make_unique<VertexArray>();
		pPositionsColorsVbo1 = std::make_unique<VertexBuffer>(mVertices.data(), sizeof(float)*mVertices.size(), bufferLayout2vec3);
		pIndexBuffer1 = std::make_unique<IndexBuffer>(mIndices.data(), mIndices.size());
		pVao1->addVertexBuffer(*pPositionsColorsVbo1);
		pVao1->setIndexBuffer(*pIndexBuffer1);
		//---------------------------------------//


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
			pShaderProgram->setMatrix4("modelMatrix", modelMatrix);

			camera.setProjectionMode(perspectiveCamera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
			pShaderProgram->setMatrix4("viewProjectionMatrix", camera.getProjectionMatrix() * camera.getViewMatrix());
			RendererOpenGL::draw(*pVao1);


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