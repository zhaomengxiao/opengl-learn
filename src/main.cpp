#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <signal.h>
#include "buffer/VertexBuffer.h"
#include "buffer/IndexBuffer.h"
#include "buffer/VertexArray.h"
#include "buffer/VertexBufferLayout.h"
#include "shader/Shader.h"
#include "Renderer.h"

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

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // 创建着色器
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    {
        // 定义正方形顶点数据（位置和颜色）
        float vertices[] = {
            // 位置              // 颜色
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // 右下
            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   // 右上
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f   // 左上
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
        layout.Push<float>(3); // 颜色属性
        va.AddBuffer(vb, layout);

        // 创建并初始化索引缓冲区
        IndexBuffer ib(indices, 6); // 6个索引

        Renderer renderer;

        while (!glfwWindowShouldClose(window))
        {
            renderer.SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            renderer.Clear();

            // 使用时间计算颜色
            float timeValue = glfwGetTime();
            float redValue = (sin(timeValue) + 1.0f) / 2.0f;
            float greenValue = (sin(timeValue + 2.094f) + 1.0f) / 2.0f; // 2.094 = 2*pi/3
            float blueValue = (sin(timeValue + 4.189f) + 1.0f) / 2.0f;  // 4.189 = 4*pi/3

            shader.SetUniform3f("u_Color", redValue, greenValue, blueValue);
            renderer.Draw(va, ib, shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
