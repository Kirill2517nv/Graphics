#pragma once
#include <memory>

#include "glm/vec3.hpp"

#include "../VertexArray.hpp"
#include "../VertexBuffer.hpp"
#include "../IndexBuffer.h"
#include "../ShaderProgram.hpp"
#include "../RendererOpenGL.hpp"
#include "../Texture2D.hpp"

namespace Engine {

	class PrimitiveObject {
	
	public:
		PrimitiveObject(glm::vec3 position);
		//PrimitiveObject(const PrimitiveObject& other) = delete;
		//PrimitiveObject(PrimitiveObject&& other) = delete;
		
		virtual void draw() = 0;
		virtual ~PrimitiveObject();
	
	protected:
		virtual void configureObject() = 0;

		glm::vec3 m_position;
		std::shared_ptr<VertexArray> m_vao = nullptr;
		std::shared_ptr<VertexBuffer> m_vbo = nullptr;
		std::shared_ptr<IndexBuffer> m_ib = nullptr;
		std::shared_ptr<ShaderProgram> m_sp = nullptr;
	private:
	
	};
}