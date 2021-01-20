#include "../pch.h"

#include "Shader.h"
#include "GL/glew.h"
#include "debug.h"

ShaderProgramSource Shader::ParseShader(const std::string &filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::stringstream ss[2];
    std::string line;
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    stream.close();
    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
    GLCall(unsigned int shader = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(shader, 1, &src, 0));
    GLCall(glCompileShader(shader));
    return shader;
}


Shader::Shader(const std::string &filePath)
: m_Path(filePath)
{
    ShaderProgramSource source = ParseShader(filePath);
    GLCall(m_RendererID = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, source.VertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);
    GLCall(glAttachShader(m_RendererID, vs));
    GLCall(glAttachShader(m_RendererID, fs));
    GLCall(glLinkProgram(m_RendererID));
    GLCall(glValidateProgram(m_RendererID));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    int location = getUniformLocation(name);
    if (location != -1)
        GLCall(glUniform4f(location, v0, v1, v2, v3));
}

int Shader::getUniformLocation(const std::string &name)
{
    if (cache.find(name) != cache.end())
        return cache[name];
    GLCall(cache[name] = glGetUniformLocation(m_RendererID, name.c_str()));
    return cache[name];
}

void Shader::setUnifromMat4f(const std::string &name, const glm::mat4 &matrix)
{
    int location = getUniformLocation(name);
    if (location != -1)
        GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));   
}

void Shader::setUniform1i(const std::string &name, int v0)
{
    int location = getUniformLocation(name);
    if (location != -1)
        GLCall(glUniform1i(location, v0));
}


