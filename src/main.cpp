#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <signal.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "buffer/VertexBuffer.h"
#include "buffer/IndexBuffer.h"
#include "buffer/VertexArray.h"
#include "buffer/VertexBufferLayout.h"
#include "shader/Shader.h"
#include "texture/Texture.h"
#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void errorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main()
{
    // 初始化GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // 启用垂直同步
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // 启用混合
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    {
        // 初始化ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 16.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // 创建着色器
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

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

        // 创建并配置顶点数组对象
        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices));
        VertexBufferLayout layout;
        layout.Push<float>(3); // 位置属性
        layout.Push<float>(2); // 纹理坐标
        va.AddBuffer(vb, layout);

        // 创建并初始化索引缓冲区
        IndexBuffer ib(indices, 6); // 6个索引

        // 创建并绑定纹理
        Texture texture("res/textures/butterfly.png");
        shader.SetUniform1i("u_Texture", 0);

        // MVP矩阵设置
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
        glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

        glm::mat4 view = glm::translate(glm::mat4(1.0f), cameraPos);
        glm::mat4 mvp = proj * view * model;
        shader.SetUniformMat4f("u_MVP", mvp);

        Renderer renderer;

        // 帧率计算变量
        double lastTime = glfwGetTime();
        int frameCount = 0;
        float fps = 0.0f;
        bool vsync = true; // 垂直同步状态

        while (!glfwWindowShouldClose(window))
        {
            renderer.SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            renderer.Clear();

            // 开始ImGui帧
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 计算帧率
            double currentTime = glfwGetTime();
            frameCount++;
            if (currentTime - lastTime >= 1.0) // 每秒更新一次
            {
                fps = static_cast<float>(frameCount);
                frameCount = 0;
                lastTime = currentTime;
            }

            // ImGui控件
            ImGui::Begin("视图控制");
            ImGui::Text("FPS: %.1f", fps);
            if (ImGui::Checkbox("垂直同步", &vsync))
            {
                glfwSwapInterval(vsync ? 1 : 0);
            }
            ImGui::Separator();
            ImGui::Text("相机位置控制");
            if (ImGui::DragFloat2("Position", &cameraPos[0], 1.0f, -400.0f, 400.0f))
            {
                view = glm::translate(glm::mat4(1.0f), glm::vec3(cameraPos[0], cameraPos[1], 0.0f));
                mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
            }
            ImGui::End();

            // 渲染场景
            texture.Bind();
            renderer.Draw(va, ib, shader);

            // 渲染ImGui
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    // 清理ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
