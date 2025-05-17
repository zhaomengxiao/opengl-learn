#pragma once
#include "Test.h"
#include "../Renderer.h"
#include "../shader/Shader.h"
#include "../texture/Texture.h"
#include "../buffer/VertexArray.h"
#include "../buffer/IndexBuffer.h"
#include "../buffer/VertexBuffer.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace test
{

    class TestTextureSquare : public Test
    {
    public:
        TestTextureSquare();
        ~TestTextureSquare();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;
        glm::mat4 m_Proj, m_View;
        glm::vec2 m_CameraPos;
        glm::vec2 m_Position1, m_Position2;
        glm::mat4 m_Model1, m_Model2;
        glm::mat4 m_MVP1, m_MVP2;
        float m_ClearColor[4];
        bool m_Vsync;
    };

}
