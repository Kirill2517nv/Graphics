#pragma once
#include "EngineCore/Rendering/OpenGL/Primitives/Cube.hpp"

namespace Engine {

	Cube::Cube(const glm::vec3& position, const float& width, const float& lenght) :
		PrimitiveObject(position),
		m_width(width),
		m_lenght(lenght) {
		configureObject();

	}

	void Cube::draw() {
		if (m_vao != nullptr) {
			// RendererOpenGL functions here
			if (m_sp != nullptr) {
				m_sp->bind();
				m_sp->setMatrix4("model_matrix", m_model_matrix);
				m_sp->setVec3("camera_position", m_camera_position);
				m_sp->setVec3("light_position", m_light_source_position);
				m_sp->setVec3("light_color", m_light_source_color);
				// set material to shader
				m_sp->setFloat("ambient_factor", m_material->m_ambient_factor);
				m_sp->setFloat("diffuse_factor", m_material->m_diffuse_factor);
				m_sp->setFloat("specular_factor", m_material->m_specular_factor);
				m_sp->setFloat("shininess", m_material->m_shininess);
			}
			else {
				LOG_ERROR("No shader");
			}
			RendererOpenGL::draw(m_vao);
		}
		else {
			LOG_ERROR("No vao");
		}
	}

	void Cube::configureObject() {
		// array object initialization here

		
		float cube_norm_uv[] = {
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

		BufferLayout buffer_layout_vec3_vec3_vec2
		{
			ShaderDataType::Float3, // vertex pos
			ShaderDataType::Float3, // normals
			ShaderDataType::Float2  // UV
		};

		m_vao = std::make_shared<VertexArray>();
		m_vbo = std::make_shared<VertexBuffer>(cube_norm_uv, sizeof(cube_norm_uv), buffer_layout_vec3_vec3_vec2);
		m_ib = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

		m_vao->addVertexBuffer(m_vbo);
		m_vao->setIndexBuffer(m_ib);

	}
}

