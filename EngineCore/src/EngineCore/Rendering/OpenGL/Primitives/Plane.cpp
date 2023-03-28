#pragma once
#include "EngineCore/Rendering/OpenGL/Primitives/Plane.hpp"

namespace Engine {
	
	Plane::Plane(const glm::vec3& position, const float& width, const float& lenght) :
		PrimitiveObject(position),
		m_width(width),
		m_lenght(lenght) {
		configureObject();
		
	}

	void Plane::draw() {
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

	void Plane::configureObject() {
		// array object initialization here
		
		//		vertex positions						normals						UV                         vertex
		float plane_norm_uv[] = {
		-m_lenght / 2.0f, -m_width / 2.0f, 0,		0,  0.f,  1.f,			0.f,		0.f,					// 0
		-m_lenght / 2.0f,  m_width / 2.0f, 0,		0,  0.f,  1.f,			m_width,	0.f,					// 1
		 m_lenght / 2.0f, -m_width / 2.0f, 0,		0,  0.f,  1.f,			0.f,		m_lenght,				// 2
		 m_lenght / 2.0f,  m_width / 2.0f, 0,		0,  0.f,  1.f,			m_width,	m_lenght,				// 3
		};

		unsigned int indices_plane[] = {
			0, 1, 2, 2, 3, 1
		};
		
		BufferLayout buffer_layout_vec3_vec3_vec2
		{
			ShaderDataType::Float3, // vertex pos
			ShaderDataType::Float3, // normals
			ShaderDataType::Float2  // UV
		};

		m_vao = std::make_shared<VertexArray>();
		m_vbo = std::make_shared<VertexBuffer>(plane_norm_uv, sizeof(plane_norm_uv), buffer_layout_vec3_vec3_vec2);
		m_ib = std::make_shared<IndexBuffer>(indices_plane, sizeof(indices_plane) / sizeof(unsigned int));

		m_vao->addVertexBuffer(m_vbo);
		m_vao->setIndexBuffer(m_ib);

	}
}

