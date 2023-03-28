#include "EngineCore/Rendering/OpenGL/Primitives/PrimitiveObject.hpp"

namespace Engine {
	PrimitiveObject::PrimitiveObject(const glm::vec3& position) :
		m_position(position),
		m_material(std::make_shared<Material>())
	{
		// TODO: add rotation
		m_model_matrix = glm::mat4( 1, 0, 0, 0,
									0, 1, 0, 0,
									0, 0, 1, 0,
									m_position[0], m_position[1], m_position[2], 1);
	}

	PrimitiveObject::~PrimitiveObject() {

	}

	void PrimitiveObject::setShaderProgram(const std::shared_ptr<ShaderProgram>& new_shader_program) {
		m_sp = new_shader_program;
	}

	void PrimitiveObject::setMaterial(const std::shared_ptr<Material>& new_material) {
		m_material = new_material;
	}
}