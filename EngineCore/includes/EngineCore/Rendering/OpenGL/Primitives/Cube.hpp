#pragma once
#include "PrimitiveObject.hpp"

namespace Engine {

	class Cube : public PrimitiveObject {
	public:
		Cube(const glm::vec3& position, const float& width, const float& lenght, const float& height);
		Cube(const Cube& other) = delete;
		Cube(Cube&& other) = delete;

		virtual void draw() override;
	protected:
		virtual void configureObject() override;

	private:
		float m_width = 1;
		float m_lenght = 1;
		float m_height = 1;
	};
}