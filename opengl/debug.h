#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <GL/glew.h>
#include <assert.h>

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#define GLCall(x) GLClearError();\
    x;\
    assert(GLLogCall(#x, __FILE__, __LINE__))
#endif
