#pragma once
#include <GL/glew.h>
#include <iostream>

// OpenGL错误处理
#define GLCall(x)   \
    GLClearError(); \
    x;              \
    assert(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

class Renderer
{
public:
    void Draw() const;
    void Clear() const;
};
