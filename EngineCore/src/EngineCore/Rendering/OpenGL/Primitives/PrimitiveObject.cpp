#include "EngineCore/Rendering/OpenGL/Primitives/PrimitiveObject.hpp"

namespace Engine {
	PrimitiveObject::PrimitiveObject(glm::vec3 position) :
		m_position(position)
	{}

	PrimitiveObject::~PrimitiveObject() {

	}
}