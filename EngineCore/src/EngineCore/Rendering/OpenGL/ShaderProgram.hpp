#pragma once

namespace Engine {

	class ShaderProgram {
	public:
		ShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc);
		ShaderProgram(ShaderProgram&&);
		ShaderProgram& operator=(ShaderProgram&&);
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		void bind() const;
		static void unbind();
		bool isCompiled() const { return mIsCompiled; }

	private:
		bool mIsCompiled = false;
		unsigned int mId = 0;

	};







}