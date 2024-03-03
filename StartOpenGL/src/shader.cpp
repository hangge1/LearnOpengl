#include "shader.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

Shader::Shader(const std::string& vertex_filepath, const std::string& fragment_filepath)
    :m_shaderProgram(0)
{
    GLuint vs = CompileShader(GetFileContext(vertex_filepath), GL_VERTEX_SHADER);
    if (!vs)
    {
        __debugbreak();
    }
    GLuint fs = CompileShader(GetFileContext(fragment_filepath), GL_FRAGMENT_SHADER);
    if (!fs)
    {
        __debugbreak();
    }

    m_shaderProgram = CreateShaderProgram(vs, fs);
    if (!m_shaderProgram)
    {
        __debugbreak();
    }
}

Shader::~Shader()
{
    glDeleteProgram(m_shaderProgram);
    m_shaderProgram = 0;
}

void Shader::Bind() const
{
    glUseProgram(m_shaderProgram);
}

void Shader::UnBind() const
{
    glUseProgram(0);
}

GLuint Shader::CompileShader(const std::string& shaderSource, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    const char* src = shaderSource.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    return shader;
}

GLuint Shader::CreateShaderProgram(GLuint vs, GLuint fs)
{
    //链接着色器
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::LINK_SHADER_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    //回收资源
    glDeleteShader(vs);
    glDeleteShader(fs);

    return shaderProgram;
}

std::string Shader::GetFileContext(const std::string& filepath)
{
    std::ifstream stream(filepath);

    if (!stream.is_open())  // 判断文件是否正常打开
    {
        return "";      
    }

    std::stringstream ss;
    std::string temp;
    while (getline(stream, temp)) // 每次读取一行到tmp中，默认以换行符作为分割条件
    {
        ss << temp << std::endl;
    }
    return ss.str();
}
