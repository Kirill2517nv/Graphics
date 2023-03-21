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
	}

	void Plane::configureObject() {
		// array object initialization here
	}
}