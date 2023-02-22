#include "ShaderProgram.h"
#include <iostream>

Renderer::ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint vertexShaderID;
	if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
	{
		std::cerr << "VERTEX SHADER compile-time eror " << std::endl;
		return;
	}

	GLuint fragmentShaderID;
	if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
	{
		std::cerr << "FRAGMENT SHADER compile-time eror " << std::endl;
		glDeleteShader(vertexShaderID);
		return;
	}

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShaderID);
	glAttachShader(m_ID, fragmentShaderID);
	glLinkProgram(m_ID);

	GLint success;
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(m_ID, 1024, nullptr, infoLog);
		std::cerr << "EROR::SHADER: Link-time eror:\n" << infoLog << std::endl;
	}
	else
	{
		m_isCompiled = true;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

}

Renderer::ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_ID);
}

void Renderer::ShaderProgram::use() const
{
	glUseProgram(m_ID);
}

Renderer::ShaderProgram& Renderer::ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
{
	glDeleteProgram(m_ID);
	m_ID = shaderProgram.m_ID;
	m_isCompiled = shaderProgram.m_isCompiled;
	shaderProgram.m_ID = 0;
	shaderProgram.m_isCompiled = false;
	return *this;
}

Renderer::ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
{
	m_ID = shaderProgram.m_ID;
	m_isCompiled = shaderProgram.m_isCompiled;
	shaderProgram.m_ID = 0;
	shaderProgram.m_isCompiled = false;
}

bool Renderer::ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
{
	shaderID = glCreateShader(shaderType);
	const char* code = source.c_str();
	glShaderSource(shaderID, 1, &code, nullptr);
	glCompileShader(shaderID);

	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
		std::cerr << "EROR::SHADER: Compile-time eror:\n" << infoLog << std::endl;
		return false;
	}
	return true;
}
