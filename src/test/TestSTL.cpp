#pragma execution_character_set("utf-8")
#include "TestSTL.h"
#include "../buffer/VertexBufferLayout.h"
#include "../buffer/VertexBuffer.h"
#include "../GLErrorHandler.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace test
{
    // Static buffer for file path input
    static char s_FilePathBuffer[256] = "res/models/cube.stl";

    TestSTL::TestSTL()
        : m_CameraPos(0.0f, 0.0f, 3.0f), m_ModelPosition(0.0f, 0.0f, 0.0f), m_ModelRotation(0.0f),
          m_LightPos(1.2f, 1.0f, 2.0f), m_ObjectColor(1.0f, 0.5f, 0.31f), m_LightColor(1.0f, 1.0f, 1.0f),
          m_AmbientColor(0.1f, 0.1f, 0.1f), m_SpecularColor(1.0f, 1.0f, 1.0f),
          m_Constant(1.0f), m_Linear(0.09f), m_Quadratic(0.032f),
          m_ObjectShininess(32.0f)
    {
        m_ClearColor[0] = 0.2f;
        m_ClearColor[1] = 0.3f;
        m_ClearColor[2] = 0.3f;
        m_ClearColor[3] = 1.0f;

        // // Initial load of a default model
        // LoadModel(s_FilePathBuffer);

        m_Shader = std::make_unique<Shader>("res/shaders/BlinnPhong.shader");
        m_Shader->Bind();
    }

    TestSTL::~TestSTL()
    {
    }

    void TestSTL::LoadModel(const std::string &filepath)
    {
        STLReader::MeshData meshData = STLReader::ReadBinarySTL(filepath);

        if (meshData.vertices.empty() || meshData.indices.empty())
        {
            std::cerr << "Failed to load model or model is empty: " << filepath << std::endl;
            // Optionally load a fallback model or clear current one
            m_Vertices.clear();
            m_Indices.clear();
            m_IndexCount = 0;
            m_VAO.reset();
            m_VBO.reset();
            m_IndexBuffer.reset();
            return;
        }

        m_Vertices = std::move(meshData.vertices);
        m_Indices = std::move(meshData.indices);
        m_IndexCount = m_Indices.size();

        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3); // Position
        layout.Push<float>(3); // Normal
        m_VAO->AddBuffer(*m_VBO, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(m_Indices.data(), m_Indices.size());
    }

    void TestSTL::OnUpdate(float deltaTime)
    {
        // Update view matrix
        m_View = glm::lookAt(m_CameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Update model matrix
        m_Model = glm::mat4(1.0f);
        m_Model = glm::translate(m_Model, m_ModelPosition);
        m_Model = glm::rotate(m_Model, glm::radians(m_ModelRotation), glm::vec3(0.0f, 1.0f, 0.0f));

        if (m_usePerspective)
        {
            m_Proj = glm::perspective(glm::radians(m_perspective_fov), m_perspective_aspect, m_perspective_near, m_perspective_far);
        }
        else
        {
            m_Proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f);
        }
    }

    void TestSTL::OnRender()
    {
        GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        GLCall(glEnable(GL_DEPTH_TEST));

        Renderer renderer;

        if (m_VAO && m_IndexBuffer && m_Shader) // Only draw if model is loaded
        {
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_Model", m_Model);
            m_Shader->SetUniformMat4f("u_View", m_View);
            m_Shader->SetUniformMat4f("u_Projection", m_Proj);
            m_Shader->SetUniformMat4f("u_NormalMatrix", glm::transpose(glm::inverse(m_Model)));

            m_Shader->SetUniform3f("u_LightPos", m_LightPos.x, m_LightPos.y, m_LightPos.z);
            m_Shader->SetUniform3f("u_ObjectColor", m_ObjectColor.x, m_ObjectColor.y, m_ObjectColor.z);
            m_Shader->SetUniform3f("u_LightColor", m_LightColor.x, m_LightColor.y, m_LightColor.z);
            m_Shader->SetUniform1f("u_Constant", m_Constant);
            m_Shader->SetUniform1f("u_Linear", m_Linear);
            m_Shader->SetUniform1f("u_Quadratic", m_Quadratic);

            m_Shader->SetUniform3f("u_ViewPos", m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
            m_Shader->SetUniform1f("u_Shininess", m_ObjectShininess);

            m_Shader->SetUniform3f("u_AmbientColor", m_AmbientColor.x, m_AmbientColor.y, m_AmbientColor.z);
            m_Shader->SetUniform3f("u_SpecularColor", m_SpecularColor.x, m_SpecularColor.y, m_SpecularColor.z);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
    }

    void TestSTL::OnImGuiRender()
    {
        Test::OnImGuiRender();

        ImGui::Text("STL Model Loader");
        ImGui::InputText("File Path", s_FilePathBuffer, sizeof(s_FilePathBuffer));
        if (ImGui::Button("Load Model"))
        {
            LoadModel(s_FilePathBuffer);
        }
        ImGui::Separator();

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

        ImGui::Text("Model");
        ImGui::DragFloat3("Position", glm::value_ptr(m_ModelPosition), 0.1f, -5.0f, 5.0f);
        ImGui::DragFloat("Rotation (Y-axis)", &m_ModelRotation, 1.0f, 0.0f, 360.0f);
        ImGui::ColorEdit3("Object Color", glm::value_ptr(m_ObjectColor));
        ImGui::DragFloat("Object Shininess", &m_ObjectShininess, 10.0f, 1.0f, 256.0f);
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_AmbientColor));
        ImGui::ColorEdit3("Specular Color", glm::value_ptr(m_SpecularColor));
        ImGui::Separator();

        ImGui::Text("Light");
        ImGui::DragFloat3("Light Position", glm::value_ptr(m_LightPos), 0.1f, -10.0f, 10.0f);
        ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightColor));
        ImGui::DragFloat("Constant", &m_Constant, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Linear", &m_Linear, 0.001f, 0.0f, 1.0f);
        ImGui::DragFloat("Quadratic", &m_Quadratic, 0.001f, 0.0f, 1.0f);
        ImGui::Separator();

        ImGui::Text("Clear Color");
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
    }
}
