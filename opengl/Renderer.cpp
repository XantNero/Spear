#include "Renderer.h"
#include "GL/glew.h"
#include "debug.h"

void Renderer::draw(const unsigned int type, const VertexArray &vao, const Shader &shader)
{
    vao.bind();
    shader.bind();
    GLCall(glDrawElements(type, vao.getIndexBuffer()->getCount(), GL_UNSIGNED_INT, 0));
}

void Renderer::enableBlending() 
{
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Renderer::setClearColor(const float r, const float g, const float b, const float a) 
{
    GLCall(glClearColor(r, g, b, a));
}

void Renderer::clear() 
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::viewport(int x, int y, int width, int height) 
{
    GLCall(glViewport(x, y, width, height));
}