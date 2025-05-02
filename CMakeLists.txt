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

# 添加可执行文件
add_executable(${PROJECT_NAME} src/main.cpp)

# 链接库
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    GLEW::GLEW
    glfw
    glm::glm
)

# 添加OpenGL库
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
