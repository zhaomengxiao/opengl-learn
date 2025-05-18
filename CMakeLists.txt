cmake_minimum_required(VERSION 3.15)

# 生成compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找依赖包
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# 添加可执行文件
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/buffer/VertexBuffer.cpp
    src/buffer/IndexBuffer.cpp
    src/buffer/VertexArray.cpp
    src/shader/Shader.cpp
    src/GLErrorHandler.cpp
    src/Renderer.cpp
    src/texture/Texture.cpp
    src/test/Test.cpp
    src/test/TestMenu.cpp
    src/test/TestTextureSquare.cpp
    src/test/TestCube.cpp # Add TestCube.cpp
)

# 添加包含路径
target_include_directories(${PROJECT_NAME} PRIVATE src/vendor src/)

# 在Windows上设置控制台子系统
if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()

# 复制shader文件到可执行文件目录
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)

# 链接库
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    GLEW::GLEW
    glfw
    glm::glm
    imgui::imgui
)

# 添加OpenGL库
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
