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

	const float textScaleS = 10;
	// cube
	GLfloat pos_norm_uv[] = {
		//    position             normal            UV                  index

		// FRONT
		-1.0f, -1.f, -1.f,    -1.f,  0.f,  0.f,     0.f, 0.f,              // 0
		-1.0f,  1.f, -1.f,    -1.f,  0.f,  0.f,     1.f, 0.f,              // 1
		-1.0f,  1.f,  1.f,    -1.f,  0.f,  0.f,     1.f, 1.f,              // 2
		-1.0f, -1.f,  1.f,    -1.f,  0.f,  0.f,     0.f, 1.f,              // 3

		// BACK                                  
		 1.0f, -1.f, -1.f,     1.f,  0.f,  0.f,     1.f, 0.f,              // 4
		 1.0f,  1.f, -1.f,     1.f,  0.f,  0.f,     0.f, 0.f,              // 5
		 1.0f,  1.f,  1.f,     1.f,  0.f,  0.f,     0.f, 1.f,              // 6
		 1.0f, -1.f,  1.f,     1.f,  0.f,  0.f,     1.f, 1.f,              // 7

		 // RIGHT
		 -1.0f,  1.f, -1.f,     0.f,  1.f,  0.f,     0.f, 0.f,              // 8
		  1.0f,  1.f, -1.f,     0.f,  1.f,  0.f,     1.f, 0.f,              // 9
		  1.0f,  1.f,  1.f,     0.f,  1.f,  0.f,     1.f, 1.f,              // 10
		 -1.0f,  1.f,  1.f,     0.f,  1.f,  0.f,     0.f, 1.f,              // 11

		 // LEFT
		 -1.0f, -1.f, -1.f,     0.f, -1.f,  0.f,     1.f, 0.f,              // 12
		  1.0f, -1.f, -1.f,     0.f, -1.f,  0.f,     0.f, 0.f,              // 13
		  1.0f, -1.f,  1.f,     0.f, -1.f,  0.f,     0.f, 1.f,              // 14
		 -1.0f, -1.f,  1.f,     0.f, -1.f,  0.f,     1.f, 1.f,              // 15

		 // TOP
		 -1.0f, -1.f,  1.f,     0.f,  0.f,  1.f,     0.f, 0.f,              // 16
		 -1.0f,  1.f,  1.f,     0.f,  0.f,  1.f,     1.f, 0.f,              // 17
		  1.0f,  1.f,  1.f,     0.f,  0.f,  1.f,     1.f, 1.f,              // 18
		  1.0f, -1.f,  1.f,     0.f,  0.f,  1.f,     0.f, 1.f,              // 19

		  // BOTTOM
		  -1.0f, -1.f, -1.f,    0.f,  0.f, -1.f,     0.f, 1.f,              // 20
		  -1.0f,  1.f, -1.f,    0.f,  0.f, -1.f,     1.f, 1.f,              // 21
		   1.0f,  1.f, -1.f,    0.f,  0.f, -1.f,     1.f, 0.f,              // 22
		   1.0f, -1.f, -1.f,    0.f,  0.f, -1.f,     0.f, 0.f,              // 23
	};
	// cube indices
	GLuint indices[] = {
		0,   1,  2,  2,  3,  0, // front
		4,   5,  6,  6,  7,  4, // back
		8,   9, 10, 10, 11,  8, // right
		12, 13, 14, 14, 15, 12, // left
		16, 17, 18, 18, 19, 16, // top
		20, 21, 22, 22, 23, 20  // bottom
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
			layout(location = 1) in vec3 vertex_normal;
			layout(location = 2) in vec2 texture_coord;
           
			uniform mat4 model_matrix;
			uniform mat4 view_projection_matrix;
			uniform int current_frame; 
           
			out vec2 tex_coord_smile;
			out vec2 tex_coord_quads;
			out vec3 frag_position;
			out vec3 frag_normal
			
			void main() {
				tex_coord_smile = texture_coord;
				tex_coord_quads = texture_coord + vec2(current_frame / 1000.f, current_frame / 1000.f);
				gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
				frag_normal = mat3(transpose(inverse(model_matrix))) * vertex_normal;
				vec4 vertex_position_world = model_matrix * vec4(vertex_position, 1.0);
				frag_position = vertex_position_world.xyz;
				gl_Position = view_projection_matrix * vertex_position_world;
			}
        )";

	const char* fragmentShader =
		R"(#version 460
			in vec2 tex_coord_smile;
			in vec2 tex_coord_quads;
			in vec3 frag_position;
			in vec3 frag_normal;
			
			layout (binding = 0) uniform sampler2D InTexture_Smile;
			layout (binding = 1) uniform sampler2D InTexture_Quads;
			
			uniform vec3 camera_position;
			uniform vec3 light_position;
			uniform vec3 light_color;
			uniform float ambient_factor;
			uniform float diffuse_factor;
			uniform float specular_factor;
			uniform float shininess;
			
			out vec4 frag_color;
			
			void main() {
				// ambient
				vec3 ambient = ambient_factor * light_color;
				
				// diffuse
				vec3 normal = normalize(frag_normal);
				vec3 light_dir = normalize(light_position - frag_position);
				vec3 diffuse = diffuse_factor * light_color * max(dot(normal, light_dir), 0.0);
				
				// specular
				vec3 view_dir = normalize(camera_position - frag_position);
				vec3 reflect_dir = reflect(-light_dir, normal);
				float specular_value = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
				vec3 specular = specular_factor * specular_value * light_color;
				
				//frag_color = texture(InTexture_Smile, tex_coord_smile) * texture(InTexture_Quads, tex_coord_quads);
				frag_color = texture(InTexture_Smile, tex_coord_smile) * vec4(ambient + diffuse + specular, 1.f);
			}
        )";

	
	// LIGHT SOURCE SHADERS
	const char* vertex_shader_light_source =
		R"(#version 460
			layout(location = 0) in vec3 vertex_position;
			layout(location = 1) in vec3 vertex_color;

			uniform mat4 model_matrix;
			uniform mat4 view_projection_matrix;
           
			void main() {
              gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position * 0.1f, 1.0);
           }
        )";

	const char* fragment_shader_light_source =
		R"(#version 460
			out vec4 frag_color;
			
			uniform vec3 light_color;
			
			void main() {
				frag_color = vec4(light_color, 1.f);
			}
        )";

	// light source cube
	std::unique_ptr<ShaderProgram> pSP_light_source;
	
	// cube
	std::unique_ptr<ShaderProgram> pShaderProgram;
	std::unique_ptr<VertexBuffer> pPositionsColorsVbo;
	std::unique_ptr<IndexBuffer> pIndexBuffer;
	std::unique_ptr<Texture2D> p_texture_smile;
	std::unique_ptr<Texture2D> p_texture_quads;
	std::unique_ptr<VertexArray> p_cube_vao;
	float scale[3] = { 1.f, 1.f, 1.f };
	float rotate = 0.f;
	float translate[3] = { 0.f, 0.f, 0.f };
	float mBackgroundColor[4] = { 0.33f, 0.33f, 0.33f, 0.f };

	std::array<glm::vec3, 5> positions = {
											glm::vec3(-2.f, -2.f, -4.f),
											glm::vec3(-5.f,  0.f,  3.f),
											glm::vec3(2.f,  1.f, -2.f),
											glm::vec3(4.f, -3.f,  3.f),
											glm::vec3(1.f, -7.f,  1.f)
										};

	Application::Application() {
		LOG_INFO("Starting application");
	}

	Application::~Application() {
		LOG_INFO("Closing application");
	}

	int Application::start(unsigned int window_width, unsigned int window_height, const char* title)
	{
		mpWindow = std::make_unique<Window>(title, window_width, window_height);
		camera.setViewportSize(static_cast<float>(window_width), static_cast<float>(window_height));

		mEventDispatcher.addEventListener<EventMouseMoved>(
			[](EventMouseMoved& event) {
				//LOG_INFO("[MouseMoved] Mouse moved to {0} x {1}", event.x, event.y);
			}
		);

		mEventDispatcher.addEventListener<EventWindowResize>(
			[&](EventWindowResize& event) {
				LOG_INFO("[WindowResized] Changed size to {0} x {1}", event.width, event.height);
				camera.setViewportSize(event.width, event.height);
				draw();
			}
		);

		mEventDispatcher.addEventListener<EventWindowClose>(
			[&](EventWindowClose& event) {
				LOG_INFO("[WindowClose]");
				close();
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

		BufferLayout bufferLayoutVec3Vec3Vec2
		{
			ShaderDataType::Float3,
			ShaderDataType::Float2
		};

		p_cube_vao = std::make_unique<VertexArray>();
		pPositionsColorsVbo = std::make_unique<VertexBuffer>(pos_norm_uv, sizeof(pos_norm_uv), bufferLayoutVec3Vec3Vec2);
		pIndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

		p_cube_vao->addVertexBuffer(*pPositionsColorsVbo);
		p_cube_vao->setIndexBuffer(*pIndexBuffer);
		//---------------------------------------//
		
		
		RendererOpenGL::enableDepthBuffer();

		while (!mbCloseWindow) {
			draw();
		}
		mpWindow = nullptr;

		return 0;
	}

	glm::vec2 Application::getCurrentCursorPosition() const {
		return mpWindow->getCurrentCursorPosition();
	}
	void Application::draw() {
		static int current_frame = 0;
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

		pShaderProgram->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
		RendererOpenGL::draw(*p_cube_vao);

		for (const glm::vec3& current_position : positions)
		{
			glm::mat4 translate_matrix(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				current_position[0], current_position[1], current_position[2], 1);
			pShaderProgram->setMatrix4("model_matrix", translate_matrix);
			RendererOpenGL::draw(*p_cube_vao);
		}

		UIModule::onUiDrawBegin();
		onUiDraw();
		UIModule::onUiDrawEnd();

		mpWindow->onUpdate();
		onUpdate();
	}
}