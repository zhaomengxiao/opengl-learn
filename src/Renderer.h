#ifndef RENDERER_H
#define RENDERER_H

#include "GLErrorHandler.h"

// 前向声明
class VertexArray;
class IndexBuffer;
class Shader;

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
    void SetClearColor(float r, float g, float b, float a) const;
};

#endif // RENDERER_H
