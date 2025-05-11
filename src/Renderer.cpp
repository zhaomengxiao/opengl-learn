#include "Renderer.h"
#include "buffer/VertexArray.h"
#include "buffer/IndexBuffer.h"
#include "shader/Shader.h"

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::SetClearColor(float r, float g, float b, float a) const
{
    GLCall(glClearColor(r, g, b, a));
}
