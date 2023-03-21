#pragma once
#include "PrimitiveObject.hpp"

namespace Engine {

	class Plane : public PrimitiveObject {
	public:
		Plane(glm::vec3 position, float width, float lenght);

		virtual void draw() override;
	protected:
		virtual void configureObject() override;

	private:
		float m_width = 1;
		float m_lenght = 1;
	};
}