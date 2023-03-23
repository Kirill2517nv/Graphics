#pragma once
#include "EngineCore/Rendering/OpenGL/Primitives/Plane.hpp"

namespace Engine {
	
	Plane::Plane(glm::vec3 position, float width, float lenght) :
		PrimitiveObject(position),
		m_width(width),
		m_lenght(lenght) {
		
	}

	void Plane::draw() {
		// RendererOpenGL functions here
		RendererOpenGL::draw(m_vao);
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
		m_ib = std::make_shared<IndexBuffer>(indices_plane, sizeof(indices_plane) / sizeof(int));

		m_vao->addVertexBuffer(m_vbo);
		m_vao->setIndexBuffer(m_ib);

	}
}