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
    class TestBlinnPhongDiffuse : public Test
    {
    public:
        TestBlinnPhongDiffuse();
        ~TestBlinnPhongDiffuse();

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
        glm::vec3 m_CubePosition;
        float m_CubeRotation;
        float m_ClearColor[4];

        // Blinn-Phong Diffuse specific
        glm::vec3 m_LightPos;
        glm::vec3 m_ObjectColor;
        glm::vec3 m_LightColor;

        bool m_usePerspective{true};
        float m_perspective_fov{45.0f};
        float m_perspective_aspect{800.0f / 600.0f};
        float m_perspective_near{0.1f};
        float m_perspective_far{100.0f};
    };
}
