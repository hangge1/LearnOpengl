#pragma once

#include <string>

#include "Renderer.h"

class Shader
{
private:
	GLuint m_shaderProgram;
public:
	Shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;
private:
	GLuint CompileShader(const std::string& shaderSource, GLenum shaderType);
	GLuint CreateShaderProgram(GLuint vs, GLuint fs);
	std::string GetFileContext(const std::string& filepath);
};

