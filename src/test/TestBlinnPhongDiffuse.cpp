#pragma execution_character_set("utf-8")
#include "TestBlinnPhongDiffuse.h"
#include "../buffer/VertexBufferLayout.h"
#include "../buffer/VertexBuffer.h"
#include "../GLErrorHandler.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace test
{
    TestBlinnPhongDiffuse::TestBlinnPhongDiffuse()
        : m_CameraPos(0.0f, 0.0f, 3.0f), m_CubePosition(0.0f, 0.0f, 0.0f), m_CubeRotation(0.0f),
          m_LightPos(1.2f, 1.0f, 2.0f), m_ObjectColor(1.0f, 0.5f, 0.31f), m_LightColor(1.0f, 1.0f, 1.0f),
          m_Constant(1.0f), m_Linear(0.09f), m_Quadratic(0.032f), // Initialize attenuation factors
          m_ObjectShininess(32.0f)
    {
        m_ClearColor[0] = 0.2f;
        m_ClearColor[1] = 0.3f;
        m_ClearColor[2] = 0.3f;
        m_ClearColor[3] = 1.0f;

        // 立方体顶点数据，包含位置和法线
        float vertices[] = {
            // Positions            // Normals
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Back face
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Front face
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, // Left face
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Right face
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // Bottom face
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top face
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

        // 对于立方体，每个面有独立的法线，所以直接使用顶点数据即可，不需要索引缓冲区
        // 如果使用索引缓冲区，需要确保每个顶点只属于一个面，或者为每个面重复顶点以提供正确的法线
        // 这里为了简化，直接使用非索引的顶点数据，共36个顶点（6个面 * 2个三角形 * 3个顶点）
        // 因此，IndexBuffer将不再需要，或者可以传入一个简单的序列索引
        unsigned int indices[] = {
            0, 1, 2, 3, 4, 5,       // Back face
            6, 7, 8, 9, 10, 11,     // Front face
            12, 13, 14, 15, 16, 17, // Left face
            18, 19, 20, 21, 22, 23, // Right face
            24, 25, 26, 27, 28, 29, // Bottom face
            30, 31, 32, 33, 34, 35  // Top face
        };

        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
        VertexBufferLayout layout;
        layout.Push<float>(3); // 位置属性
        layout.Push<float>(3); // 法线属性
        m_VAO->AddBuffer(*m_VBO, layout);

        // 对于非索引绘制，IndexBuffer可以简单地包含0到35的序列
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);
        m_Shader = std::make_unique<Shader>("res/shaders/BlinnPhongDiffuse.shader");

        m_Shader->Bind();
    }

    TestBlinnPhongDiffuse::~TestBlinnPhongDiffuse()
    {
    }

    void TestBlinnPhongDiffuse::OnUpdate(float deltaTime)
    {
        // 更新视图矩阵
        m_View = glm::lookAt(m_CameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // 更新模型矩阵
        m_Model = glm::mat4(1.0f);
        m_Model = glm::translate(m_Model, m_CubePosition);
        m_Model = glm::rotate(m_Model, glm::radians(m_CubeRotation), glm::vec3(0.0f, 1.0f, 0.0f));

        if (m_usePerspective)
        {
            m_Proj = glm::perspective(glm::radians(m_perspective_fov), m_perspective_aspect, m_perspective_near, m_perspective_far);
        }
        else
        {
            // 正交投影参数需要根据实际窗口大小调整，这里暂时使用一个示例值
            m_Proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f);
        }
    }

    void TestBlinnPhongDiffuse::OnRender()
    {
        GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        GLCall(glEnable(GL_DEPTH_TEST));

        Renderer renderer;

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_Model", m_Model);
        m_Shader->SetUniformMat4f("u_View", m_View);
        m_Shader->SetUniformMat4f("u_Projection", m_Proj);
        m_Shader->SetUniformMat4f("u_NormalMatrix", glm::transpose(glm::inverse(m_Model))); // 计算法线矩阵

        m_Shader->SetUniform3f("u_LightPos", m_LightPos.x, m_LightPos.y, m_LightPos.z);
        m_Shader->SetUniform3f("u_ObjectColor", m_ObjectColor.x, m_ObjectColor.y, m_ObjectColor.z);
        m_Shader->SetUniform3f("u_LightColor", m_LightColor.x, m_LightColor.y, m_LightColor.z);
        m_Shader->SetUniform1f("u_Constant", m_Constant);
        m_Shader->SetUniform1f("u_Linear", m_Linear);
        m_Shader->SetUniform1f("u_Quadratic", m_Quadratic);

        m_Shader->SetUniform3f("u_ViewPos", m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
        m_Shader->SetUniform1f("u_Shininess", m_ObjectShininess);

        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

    void TestBlinnPhongDiffuse::OnImGuiRender()
    {
        Test::OnImGuiRender();

        ImGui::Text("Camera");
        ImGui::DragFloat3("Camera Position", glm::value_ptr(m_CameraPos), 0.1f, -10.0f, 10.0f);
        ImGui::Checkbox("Perspective", &m_usePerspective);
        if (m_usePerspective)
        {
            ImGui::DragFloat("FOV", &m_perspective_fov, 1.0f, 1.0f, 179.0f);
            ImGui::DragFloat("Aspect", &m_perspective_aspect, 0.01f, 0.1f, 10.0f);
            ImGui::DragFloat("Near", &m_perspective_near, 0.01f, 0.01f, 100.0f);
            ImGui::DragFloat("Far", &m_perspective_far, 0.1f, 1.0f, 100.0f);
        }
        ImGui::Separator();

        ImGui::Text("Cube");
        ImGui::DragFloat3("Position", glm::value_ptr(m_CubePosition), 0.1f, -5.0f, 5.0f);
        ImGui::DragFloat("Rotation (Y-axis)", &m_CubeRotation, 1.0f, 0.0f, 360.0f);
        ImGui::Separator();

        ImGui::Text("Light");
        ImGui::DragFloat3("Light Position", glm::value_ptr(m_LightPos), 0.1f, -10.0f, 10.0f);
        ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightColor));
        ImGui::DragFloat("Constant", &m_Constant, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Linear", &m_Linear, 0.001f, 0.0f, 1.0f);
        ImGui::DragFloat("Quadratic", &m_Quadratic, 0.001f, 0.0f, 1.0f);
        ImGui::Separator();

        ImGui::Text("Object");
        ImGui::ColorEdit3("Object Color", glm::value_ptr(m_ObjectColor));
        ImGui::DragFloat("Object Shininess", &m_ObjectShininess, 10.0f, 1.0f, 256.0f);
        ImGui::Separator();

        ImGui::Text("Clear Color");
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
    }
}
