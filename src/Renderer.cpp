#include "Renderer.h"
#include <assert.h>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] (" << error << "): " << function
                  << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Draw() const
{
    // TODO: 实现渲染逻辑
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
