#pragma once
#include <memory>

#include "glm/vec3.hpp"

#include "../VertexArray.hpp"
#include "../VertexBuffer.hpp"
#include "../IndexBuffer.h"
#include "../ShaderProgram.hpp"
#include "../RendererOpenGL.hpp"
#include "../Texture2D.hpp"
#include "../Material.hpp"

#include "EngineCore/Log.h"

namespace Engine {

	class PrimitiveObject {
	
	public:
		PrimitiveObject(const glm::vec3& position);
		PrimitiveObject(const PrimitiveObject& other) = delete;
		PrimitiveObject(PrimitiveObject&& other) = delete;
		
		virtual void draw() = 0;
		virtual ~PrimitiveObject();
	
		virtual void setShaderProgram(const std::shared_ptr<ShaderProgram>& new_shader_program);
		virtual void setMaterial(const std::shared_ptr<Material>& new_material);

		virtual void setCameraPosition(const glm::vec3& camera_pos) { m_camera_position = camera_pos; };
		virtual void setLightSourcePosition(const glm::vec3& light_source_pos) { m_light_source_position = light_source_pos; };
		virtual void setLightSourceColor(const glm::vec3& light_source_color) { m_light_source_color = light_source_color; };

		virtual void setModelMatrix(const glm::mat4& new_model_matrix) { m_model_matrix = new_model_matrix; };

	protected:
		virtual void configureObject() = 0;

		glm::vec3 m_position;
		std::shared_ptr<VertexArray> m_vao = nullptr;
		std::shared_ptr<VertexBuffer> m_vbo = nullptr;
		std::shared_ptr<IndexBuffer> m_ib = nullptr;
		std::shared_ptr<ShaderProgram> m_sp = nullptr;
		std::shared_ptr<Material> m_material;
		glm::vec3 m_light_source_position = { 0.f, 0.f, 0.f };
		glm::vec3 m_light_source_color = { 1.f, 1.f, 1.f };
		glm::vec3 m_camera_position = { 0.f, 0.f, 0.f };

		glm::mat4 m_model_matrix;
	private:
	
	};
}