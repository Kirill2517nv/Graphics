#include "EngineCore/Rendering/OpenGL/ShaderProgram.hpp"

#include "EngineCore/Log.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace Engine {
	bool createShader(const char* source, const GLenum shderType, GLuint& shaderId) {
		shaderId = glCreateShader(shderType);
		glShaderSource(shaderId, 1, &source, nullptr);
		glCompileShader(shaderId);

		GLint success;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			char infoLog[1024];
			glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);

			LOG_CRITICAL("Shader complilation error:\n{0}", infoLog);
			return false;
		}

		return true;
	}

	ShaderProgram::ShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc) {
		GLuint vertexShaderId = 0;
		if (!createShader(vertexShaderSrc, GL_VERTEX_SHADER, vertexShaderId)) {
			LOG_CRITICAL("VERTEX SHADER: compile-time error!");
			glDeleteShader(vertexShaderId);
			return;
		}

		GLuint fragmentShaderId = 0;
		if (!createShader(fragmentShaderSrc, GL_FRAGMENT_SHADER, fragmentShaderId)) {
			LOG_CRITICAL("FRAGMENT SHADER: compile-time error!");
			glDeleteShader(vertexShaderId);
			glDeleteShader(fragmentShaderId);
			return;
		}

		mId = glCreateProgram();
		glAttachShader(mId, vertexShaderId);
		glAttachShader(mId, fragmentShaderId);
		glLinkProgram(mId);

		GLint success;
		glGetProgramiv(mId, GL_LINK_STATUS, &success);
		if (success == GL_FALSE) {
			GLchar infoLog[1024];
			glGetProgramInfoLog(mId, 1024, nullptr, infoLog);

			LOG_CRITICAL("SHADER PROGRAM: link time error:\n{0}", infoLog);
			glDeleteProgram(mId);
			mId = 0;
			glDeleteShader(vertexShaderId);
			glDeleteShader(fragmentShaderId);

			return;
		}
		else {
			mIsCompiled = true;
		}

		glDetachShader(mId, vertexShaderId);
		glDetachShader(mId, fragmentShaderId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(mId);
	}

	void ShaderProgram::bind() const {
		glUseProgram(mId);
	}

	void ShaderProgram::unbind() {
		glUseProgram(0);
	}

	void ShaderProgram::setMatrix4(const char* name, const glm::mat4x4 matrix) {
		glUniformMatrix4fv(glGetUniformLocation(mId, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::setVec3(const char* name, const glm::vec3 vec) const
	{
		glUniform3f(glGetUniformLocation(mId, name), vec.x, vec.y, vec.z);
	}

	void ShaderProgram::setInt(const char* name, const int value) const
	{
		glUniform1i(glGetUniformLocation(mId, name), value);
	}

	void ShaderProgram::setFloat(const char* name, const float value) const
	{
		glUniform1f(glGetUniformLocation(mId, name), value);
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept {
		glDeleteProgram(mId);
		mId = shaderProgram.mId;
		mIsCompiled = shaderProgram.mIsCompiled;

		shaderProgram.mId = 0;
		shaderProgram.mIsCompiled = false;
		return *this;
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept {
		mId = shaderProgram.mId;
		mIsCompiled = shaderProgram.mIsCompiled;

		shaderProgram.mId = 0;
		shaderProgram.mIsCompiled = false;
	}

}