#include <iostream>


#include "EngineCore/Application.h"
#include "EngineCore/Log.h"
#include "EngineCore/Window.h"
#include "EngineCore/Event.hpp"
#include "EngineCore/Input.hpp"


#include "EngineCore/Rendering/OpenGL/ShaderContainer.hpp"
#include "EngineCore/Rendering/OpenGL/Primitives/Plane.hpp"
#include "EngineCore/Rendering/OpenGL/Primitives/Sphere.hpp"

#include "EngineCore/Modules/UIModule.hpp"

#include <imgui/imgui.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>

namespace Engine {

	const float textScaleS = 10;

	float pos_norm_uv[] = {
		//    position             normal            UV                  index

		// FRONT
		-1.0f, -1.f, -1.f,     -1.f,  0.f,  0.f,     0.f, 0.f,              // 0
		-1.0f,  1.f, -1.f,     -1.f,  0.f,  0.f,     1.f, 0.f,              // 1
		-1.0f,  1.f,  1.f,     -1.f,  0.f,  0.f,     1.f, 1.f,              // 2
		-1.0f, -1.f,  1.f,     -1.f,  0.f,  0.f,     0.f, 1.f,              // 3

		// BACK                                  
		 1.0f, -1.f, -1.f,		1.f,  0.f,  0.f,     1.f, 0.f,              // 4
		 1.0f,  1.f, -1.f,		1.f,  0.f,  0.f,     0.f, 0.f,              // 5
		 1.0f,  1.f,  1.f,		1.f,  0.f,  0.f,     0.f, 1.f,              // 6
		 1.0f, -1.f,  1.f,		1.f,  0.f,  0.f,     1.f, 1.f,              // 7

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
	unsigned int indices[] = {
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
				data[3 * (x + width * y) + 0] = 255;
				data[3 * (x + width * y) + 1] = 255;
				data[3 * (x + width * y) + 2] = 255;
			}
		}
		//// face
		//generate_circle(data, width, height, width * 0.5, height * 0.5, width * 0.4, 255, 255, 0);
		//// smile
		//generate_circle(data, width, height, width * 0.5, height * 0.4, width * 0.2, 0, 0, 0);
		//generate_circle(data, width, height, width * 0.5, height * 0.45, width * 0.2, 255, 255, 0);
		//// eyes
		//generate_circle(data, width, height, width * 0.35, height * 0.6, width * 0.07, 255, 0, 255);
		//generate_circle(data, width, height, width * 0.65, height * 0.6, width * 0.07, 0, 0, 255);
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
					data[3 * (x + width * y) + 1] = 0;
					data[3 * (x + width * y) + 2] = 220;
				}
			}
		}
	}
	//---------------------------------------//

	
	std::shared_ptr<ShaderProgram> pSP_light_source; // light source shader
	std::shared_ptr<ShaderProgram> pSP_basic; // basic shader with simple lightning
	
	// materials
	std::shared_ptr<Material> basic_material;
	std::shared_ptr<Material> plane_material;

	// cube
	std::shared_ptr<VertexBuffer> pPositionsColorsVbo;
	std::shared_ptr<IndexBuffer> pIndexBuffer;
	std::shared_ptr<Texture2D> p_texture_smile;
	std::shared_ptr<Texture2D> p_texture_quads;
	std::shared_ptr<VertexArray> p_cube_vao;

	// Example Plane for
	std::shared_ptr<Plane> example_plane;
	std::shared_ptr<Sphere> example_sphere;

	float scale[3] = { 1.f, 1.f, 1.f };
	float rotate = 0.f;
	float translate[3] = { 0.f, 0.f, 0.f };
	float mBackgroundColor[4] = { 0.33f, 0.33f, 0.33f, 0.f };

	// origin positions for example cubes
	std::array<glm::vec3, 5> positions = {
											glm::vec3(0, 0,	 2.f),
											glm::vec3(0, 0,  4.f),
											glm::vec3(0, 0,  6.f),
											glm::vec3(0, 0,  8.f),
											glm::vec3(0, 0,  10.f)
										};

	Application::Application() {
		LOG_INFO("Starting application");
	}

	Application::~Application() {
		LOG_INFO("Closing application");
	}

	int Application::start(unsigned int window_width, unsigned int window_height, const char* title)
	{	
		// making a window
		mpWindow = std::make_shared<Window>(title, window_width, window_height);
		// setting up a camera viewport
		camera.setViewportSize(static_cast<float>(window_width), static_cast<float>(window_height));
		
		// Events ( add new event callback here and event itself in Event class)
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
		// Mouse event
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

		// various stuff initialization

		const unsigned int width = 1000;
		const unsigned int height = 1000;
		const unsigned int channels = 3;
		
		// textures generation
		auto* data = new unsigned char[width * height * channels];
		generate_smile_texture(data, width, height);
		
		p_texture_smile = std::make_shared<Texture2D>(data, width, height);
		p_texture_smile->bind(0);
		
		generate_quads_texture(data, width, height);
		p_texture_quads = std::make_shared<Texture2D>(data, width, height);
		p_texture_quads->bind(1);

		delete[] data; // cleaning up texture data
		
		// Compiling shader programs
		
		// basic shader program with simple lighning
		pSP_basic = std::make_shared<ShaderProgram>(Shaders::VERTEX_SHADER_WITH_BASIC_LIGHTNING, Shaders::FRAGMENT_SHADER_WITH_BASIC_LIGHTNING);
		if (!pSP_basic->isCompiled())
		{
			return false;
		}

		// Light source shader program
		pSP_light_source = std::make_shared<ShaderProgram>(Shaders::vertex_shader_light_source, Shaders::fragment_shader_light_source);
		if (!pSP_light_source->isCompiled())
		{
			return false;
		}

		// material init
		basic_material = std::make_shared<Material>(ambient_factor, diffuse_factor, specular_factor, shininess);
		plane_material = std::make_shared<Material>(ambient_factor, diffuse_factor, 2* specular_factor, 2* shininess);

		// ---------  delete
		BufferLayout bufferLayoutVec3Vec3Vec2
		{
			ShaderDataType::Float3,
			ShaderDataType::Float3,
			ShaderDataType::Float2
		};
		
		//---------------------------------------//

		// cube buffers initialization
		p_cube_vao = std::make_shared<VertexArray>();
		pPositionsColorsVbo = std::make_shared<VertexBuffer>(pos_norm_uv, sizeof(pos_norm_uv), bufferLayoutVec3Vec3Vec2);
		pIndexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

		p_cube_vao->addVertexBuffer(pPositionsColorsVbo);
		p_cube_vao->setIndexBuffer(pIndexBuffer);
		//---------------------------------------//
		
		// Initializing plane
		example_plane = std::make_shared<Plane>(glm::vec3(0, 0, 0), 100.f, 100.f);
		example_plane->setMaterial(plane_material);
		example_plane->setShaderProgram(pSP_basic);
		// Initializing sphere
		example_sphere = std::make_shared<Sphere>(glm::vec3(10, 0, 2), 1.f);
		example_sphere->setMaterial(plane_material);
		example_sphere->setShaderProgram(pSP_basic);

		RendererOpenGL::enableDepthBuffer();
		
		// main cycle
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
		
		// view_projection matrix
		glm::mat4 view_projection_matrix = camera.getProjectionMatrix() * camera.getViewMatrix();

		// activating basic shader
		pSP_basic->bind();
		pSP_basic->setVec3("camera_position", camera.getPosition());
		pSP_basic->setVec3("light_position", glm::vec3(light_source_pos[0], light_source_pos[1], light_source_pos[2]));
		pSP_basic->setVec3("light_color", glm::vec3(ls_brightness * light_source_color[0], ls_brightness * light_source_color[1], ls_brightness * light_source_color[2]));
		pSP_basic->setFloat("ambient_factor", ambient_factor);
		pSP_basic->setFloat("diffuse_factor", diffuse_factor);
		pSP_basic->setFloat("specular_factor", specular_factor);
		pSP_basic->setFloat("shininess", shininess);
		
		/*glm::mat4 scaleMatrix(scale[0], 0, 0, 0,
			0, scale[1], 0, 0,
			0, 0, scale[2], 0,
			0, 0, 0, 1);

		float rotateInRadians = glm::radians(rotate);
		glm::mat4 rotateMatrix(cos(rotateInRadians), sin(rotateInRadians), 0, 0,
			-sin(rotateInRadians), cos(rotateInRadians), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		*/
		glm::mat4 translateMatrix(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			translate[0], translate[1], translate[2], 1);

		glm::mat4 modelMatrix = translateMatrix;
		

		// rendering cubes
		pSP_basic->setMatrix4("model_matrix", modelMatrix);

		pSP_basic->setMatrix4("view_projection_matrix", view_projection_matrix);
		
		unsigned int scale = 1;
		for (const glm::vec3& current_position : positions)
		{
			glm::mat4 translate_matrix(scale, 0, 0, 0,
				0, scale, 0, 0,
				0, 0, scale , 0,
				current_position[0] * scale, current_position[1] * scale, current_position[2] * scale, 1);
			pSP_basic->setMatrix4("model_matrix", translate_matrix);
			RendererOpenGL::draw(p_cube_vao);
			scale*=2;
		}
		// rendering plane
		example_plane->setCameraPosition(camera.getCameraPosition());
		example_plane->setLightSourcePosition(glm::vec3(light_source_pos[0], light_source_pos[1], light_source_pos[2]));
		example_plane->setLightSourceColor(glm::vec3(ls_brightness * light_source_color[0], ls_brightness * light_source_color[1], ls_brightness * light_source_color[2]));
		example_plane->draw();
		// rendering sphere
		example_sphere->setCameraPosition(camera.getCameraPosition());
		example_sphere->setLightSourcePosition(glm::vec3(light_source_pos[0], light_source_pos[1], light_source_pos[2]));
		example_sphere->setLightSourceColor(glm::vec3(ls_brightness * light_source_color[0], ls_brightness * light_source_color[1], ls_brightness * light_source_color[2]));
		example_sphere->draw();

		// rendering light source cube
		{
			pSP_light_source->bind();
			pSP_light_source->setMatrix4("view_projection_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());
			glm::mat4 translate_matrix(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				light_source_pos[0], light_source_pos[1], light_source_pos[2], 1);
			pSP_light_source->setMatrix4("model_matrix", translate_matrix);
			pSP_light_source->setVec3("light_color", glm::vec3(light_source_color[0], light_source_color[1], light_source_color[2]));
			RendererOpenGL::draw(p_cube_vao);
		}

		UIModule::onUiDrawBegin();
		onUiDraw();
		UIModule::onUiDrawEnd();

		mpWindow->onUpdate();
		onUpdate();
	}
}