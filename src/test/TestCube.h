#pragma once
#include "Test.h"
#include "../Renderer.h"
#include "../shader/Shader.h"
#include "../buffer/VertexArray.h"
#include "../buffer/IndexBuffer.h"
#include "../buffer/VertexBuffer.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace test
{
    class TestCube : public Test
    {
    public:
        TestCube();
        ~TestCube();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        glm::mat4 m_Proj, m_View, m_Model;
        glm::vec3 m_CameraPos;
        float m_CubeRotation;
        glm::vec3 m_CubePosition;
        float m_ClearColor[4];
        bool m_Vsync{true};
        bool m_usePerspective{true};
    };
}
