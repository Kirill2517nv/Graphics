#pragma once
#include "PrimitiveObject.hpp"

namespace Engine {

	class Plane : public PrimitiveObject {
	public:
		Plane(const glm::vec3& position, const float& width, const float& lenght);
		Plane(const Plane& other) = delete;
		Plane(Plane&& other) = delete;

		virtual void draw() override;
	protected:
		virtual void configureObject() override;

	private:
		float m_width = 1;
		float m_lenght = 1;
	};
}