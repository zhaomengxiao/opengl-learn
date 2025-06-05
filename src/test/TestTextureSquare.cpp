#include "TestTextureSquare.h"
#include "../buffer/VertexBufferLayout.h"
#include "../buffer/VertexBuffer.h"
#include "../GLErrorHandler.h"
#include "imgui.h"
#include <GLFW/glfw3.h>

namespace test
{
    TestTextureSquare::TestTextureSquare()
        : m_CameraPos(0.0f, 0.0f), m_Position1(0.0f, 0.0f), m_Position2(0.0f, 0.0f),
          m_Proj(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f)),
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(m_CameraPos.x, m_CameraPos.y, 0.0f))),
          m_Vsync(true)
    {
        m_ClearColor[0] = 0.2f;
        m_ClearColor[1] = 0.3f;
        m_ClearColor[2] = 0.3f;
        m_ClearColor[3] = 1.0f;

        // 定义正方形顶点数据（位置和纹理坐标）
        float vertices[] = {
            // 位置             // 纹理坐标
            400.0f, 300.0f, 0.0f, 0.0f, 0.0f, // 左下
            600.0f, 300.0f, 0.0f, 1.0f, 0.0f, // 右下
            600.0f, 500.0f, 0.0f, 1.0f, 1.0f, // 右上
            400.0f, 500.0f, 0.0f, 0.0f, 1.0f  // 左上
        };

        // 定义索引数据
        unsigned int indices[] = {
            0, 1, 2, // 第一个三角形
            2, 3, 0  // 第二个三角形
        };

        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
        VertexBufferLayout layout;
        layout.Push<float>(3); // 位置属性
        layout.Push<float>(2); // 纹理坐标
        m_VAO->AddBuffer(*m_VBO, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Texture = std::make_unique<Texture>("res/textures/butterfly.png");

        m_Shader->Bind();
        m_Shader->SetUniform1i("u_Texture", 0);
    }

    TestTextureSquare::~TestTextureSquare()
    {
    }

    void TestTextureSquare::OnUpdate(float deltaTime)
    {
        // 更新视图矩阵
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(m_CameraPos.x, m_CameraPos.y, 0.0f));
        // 更新两个物体的MVP矩阵
        m_Model1 = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position1.x, m_Position1.y, 0.0f));
        m_Model2 = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position2.x, m_Position2.y, 0.0f));
        m_MVP1 = m_Proj * m_View * m_Model1;
        m_MVP2 = m_Proj * m_View * m_Model2;
    }

    void TestTextureSquare::OnRender()
    {
        GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        // 渲染物体1
        m_Texture->Bind();
        m_Shader->SetUniformMat4f("u_MVP", m_MVP1);
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

        // 渲染物体2
        m_Shader->SetUniformMat4f("u_MVP", m_MVP2);
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

    void TestTextureSquare::OnImGuiRender()
    {
        Test::OnImGuiRender();

        if (ImGui::Checkbox("垂直同步", &m_Vsync))
        {
            glfwSwapInterval(m_Vsync ? 1 : 0);
        }
        ImGui::Separator();

        // 相机控制
        ImGui::Text("相机控制");
        ImGui::DragFloat2("相机位置", &m_CameraPos.x, 1.0f, -400.0f, 400.0f);

        // 物体1控制
        ImGui::Text("物体1控制");
        ImGui::DragFloat2("位置##1", &m_Position1.x, 1.0f, -400.0f, 400.0f);

        // 物体2控制
        ImGui::Text("物体2控制");
        ImGui::DragFloat2("位置##2", &m_Position2.x, 1.0f, -400.0f, 400.0f);
    }
}
