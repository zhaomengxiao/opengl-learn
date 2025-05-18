#pragma execution_character_set("utf-8")
#include "TestCube.h"
#include "../buffer/VertexBufferLayout.h"
#include "../buffer/VertexBuffer.h"
#include "../GLErrorHandler.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace test
{
    TestCube::TestCube()
        : m_CameraPos(0.0f, 0.0f, 3.0f), m_CubeRotation(0.0f), m_CubePosition(0.0f, 0.0f, 0.0f)
    {
        m_ClearColor[0] = 0.2f;
        m_ClearColor[1] = 0.3f;
        m_ClearColor[2] = 0.3f;
        m_ClearColor[3] = 1.0f;

        // 8个顶点，每个顶点包含位置和颜色
        float vertices[] = {
            // Positions          // Colors
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 0: 左下后
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // 1: 右下后
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,   // 2: 右上后
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  // 3: 左上后
            -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,  // 4: 左下前
            0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // 5: 右下前
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,    // 6: 右上前
            -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f    // 7: 左上前
        };

        // 立方体索引数据
        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0, // 后面
            4, 5, 6, 6, 7, 4, // 前面
            7, 3, 0, 0, 4, 7, // 左面
            5, 1, 2, 2, 6, 5, // 右面
            3, 2, 6, 6, 7, 3, // 上面
            0, 1, 5, 5, 4, 0  // 下面
        };

        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
        VertexBufferLayout layout;
        layout.Push<float>(3); // 位置属性
        layout.Push<float>(3); // 颜色属性
        m_VAO->AddBuffer(*m_VBO, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);
        m_Shader = std::make_unique<Shader>("res/shaders/Cube.shader"); // 使用新的Cube.shader

        m_Shader->Bind();

        // 设置透视投影矩阵
        // m_Proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    }

    TestCube::~TestCube()
    {
    }

    void TestCube::OnUpdate(float deltaTime)
    {
        // 更新视图矩阵
        m_View = glm::lookAt(m_CameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // 更新模型矩阵
        m_Model = glm::mat4(1.0f);
        m_Model = glm::translate(m_Model, m_CubePosition);
        m_Model = glm::rotate(m_Model, glm::radians(m_CubeRotation), glm::vec3(0.0f, 1.0f, 0.0f));

        if (m_usePerspective)
        {
            m_Proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f); // 使用透视投影
        }
        else
        {
            m_Proj = glm::ortho(-800.0f / 600.0f, 800.0f / 600.0f, -1.0f, 1.0f, 0.1f, 100.0f); // 使用正交投影
        }
    }

    void TestCube::OnRender()
    {
        GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        GLCall(glEnable(GL_DEPTH_TEST));

        Renderer renderer;

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", m_Proj * m_View * m_Model);
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

    void TestCube::OnImGuiRender()
    {
        Test::OnImGuiRender();
        ImGui::Begin("test");

        ImGui::Text("camera");
        ImGui::DragFloat3("camera position", glm::value_ptr(m_CameraPos), 0.1f, -10.0f, 10.0f);
        ImGui::Checkbox("Perspective", &m_usePerspective);
        ImGui::Separator();

        ImGui::Text("cube");
        ImGui::DragFloat3("position", glm::value_ptr(m_CubePosition), 0.1f, -5.0f, 5.0f);
        ImGui::DragFloat("rotation (Y-axis)", &m_CubeRotation, 1.0f, 0.0f, 360.0f);

        ImGui::End();
    }
}
