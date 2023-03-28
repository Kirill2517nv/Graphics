#pragma once

#include <glm/mat4x4.hpp>

namespace Engine {

	class ShaderProgram {
	public:
		ShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc);
		ShaderProgram(ShaderProgram&&) noexcept;
		ShaderProgram& operator=(ShaderProgram&&) noexcept;
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		void bind() const;
		static void unbind();
		bool isCompiled() const { return mIsCompiled; };
		void setMatrix4(const char* name, const glm::mat4x4 matrix);
		void setVec3(const char* name, const glm::vec3 vec) const;
		void setInt(const char* name, const int value) const;
		void setFloat(const char* name, const float value) const;

	private:
		bool mIsCompiled = false;
		unsigned int mId = 0;

	};
}