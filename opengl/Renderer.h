#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "VertexArray.h"
#include "Shader.h"

class Renderer 
{
public:
    static void draw(const unsigned int type, const VertexArray &, const  Shader &);
    static void enableBlending();
    static void setClearColor(const float r, const float g, const float b, const float a);
    static void clear();
    static void viewport(int x, int y, int width, int height);
};


#endif