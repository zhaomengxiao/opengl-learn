#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <signal.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "GLErrorHandler.h"
#include "test/Test.h"
#include "test/TestMenu.h"
#include "test/TestTextureSquare.h"
#include "test/TestCube.h"
#include "test/TestBlinnPhong.h"
#pragma execution_character_set("utf-8")

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

        test::TestMenu *testMenu = new test::TestMenu();
        test::Test *currentTest = testMenu;

        // 注册测试
        testMenu->RegisterTest<test::TestTextureSquare>("两个纹理方块");
        testMenu->RegisterTest<test::TestCube>("立方体");
        testMenu->RegisterTest<test::TestBlinnPhong>("Blinn-Phong Diffuse"); // Register the new test
        // 帧率计算变量
        double lastTime = glfwGetTime();
        int frameCount = 0;

        // 主循环
        while (!glfwWindowShouldClose(window))
        {
            // 计算帧率
            double currentTime = glfwGetTime();
            frameCount++;
            if (currentTime - lastTime >= 1.0) // 每秒更新一次帧率
            {
                float fps = float(frameCount) / float(currentTime - lastTime);
                if (currentTest)
                {
                    currentTest->SetFramerate(fps);
                }
                frameCount = 0;
                lastTime = currentTime;
            }

            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            // 开始ImGui帧
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest)
            {
                // 如果当前是TestMenu且有选择的测试，使用选择的测试
                test::TestMenu *menu = dynamic_cast<test::TestMenu *>(currentTest);
                test::Test *activeTest = menu ? menu->GetCurrentTest() : currentTest;

                if (activeTest)
                {
                    activeTest->OnUpdate(0.0f);
                    activeTest->OnRender();
                }

                ImGui::Begin("测试");
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            // 渲染ImGui
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        if (currentTest != testMenu)
            delete currentTest;
        delete testMenu;
    }

    // 清理ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
