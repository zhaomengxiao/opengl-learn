#pragma once
#include "Test.h"
#include "../Renderer.h"
#include "../shader/Shader.h"
#include "../buffer/VertexArray.h"
#include "../buffer/IndexBuffer.h"
#include "../buffer/VertexBuffer.h"
#include "../mesh/STLReader.h" // Include the custom STL reader
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace test
{
    class TestSTL : public Test
    {
    public:
        TestSTL();
        ~TestSTL();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        void LoadModel(const std::string &filepath); // Declare LoadModel

        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        glm::mat4 m_Proj, m_View, m_Model;
        glm::vec3 m_CameraPos;
        glm::vec3 m_ModelPosition; // Changed from m_CubePosition
        float m_ModelRotation;     // Changed from m_CubeRotation
        float m_ClearColor[4];

        // Blinn-Phong Diffuse specific
        glm::vec3 m_LightPos;
        glm::vec3 m_ObjectColor;
        glm::vec3 m_LightColor;
        glm::vec3 m_AmbientColor;
        glm::vec3 m_SpecularColor;
        float m_ObjectShininess;

        // light attenuation
        float m_Constant;
        float m_Linear;
        float m_Quadratic;

        bool m_usePerspective{true};
        float m_perspective_fov{45.0f};
        float m_perspective_aspect{800.0f / 600.0f};
        float m_perspective_near{0.1f};
        float m_perspective_far{100.0f};

        // STL specific members
        std::vector<float> m_Vertices; // Combined positions and normals
        std::vector<unsigned int> m_Indices;
        size_t m_IndexCount;
    };
}
