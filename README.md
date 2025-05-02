# OpenGL CMake Project Template

基于 CMake 的 OpenGL 项目模板，支持 Windows 平台，使用 vcpkg 管理依赖。

## 功能特点

- 使用 CMake 和 vcpkg 进行项目管理和依赖控制
- 预配置的 VS Code 开发环境
- 基于 CMake 预设的构建配置
- 支持 Debug 和 Release 模式
- 包含基础的 OpenGL 示例程序

## 依赖要求

- Windows 10 或更高版本
- [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/) (带"使用 C++的桌面开发"工作负载)
- [CMake 3.15+](https://cmake.org/download/)
- [vcpkg](https://github.com/microsoft/vcpkg)
- [VS Code](https://code.visualstudio.com/) (可选，但推荐)
- Git

## 环境配置步骤

### 1. 克隆仓库

```bash
git clone <repository-url>
cd OpenGL
```

### 2. 安装 vcpkg（如果未安装）

```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
```

### 3. 安装项目依赖

```bash
vcpkg install glfw3:x64-windows
vcpkg install glew:x64-windows
vcpkg install glm:x64-windows
vcpkg integrate install
```

### 4. 安装 VS Code 扩展（如果使用 VS Code）

- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)

## 构建项目

### 使用 CMake 命令行

```bash
# Debug构建
cmake --preset windows-debug .
cmake --build --preset debug

# Release构建
cmake --preset windows-release .
cmake --build --preset release
```

### 使用 VS Code

1. 打开项目文件夹
2. 选择 CMake 工具栏中的构建配置（Debug/Release）
3. 点击构建按钮或按 F7

## 运行程序

### 命令行运行

```bash
# Debug版本
./build/debug/Debug/OpenGLProject.exe

# Release版本
./build/release/Release/OpenGLProject.exe
```

### VS Code 调试

1. 选择运行和调试面板
2. 选择"OpenGL Debug"配置
3. 按 F5 启动调试

## 项目结构

```
OpenGL/
├── .vscode/               # VS Code配置文件
│   ├── settings.json      # CMake和编辑器设置
│   ├── launch.json        # 调试配置
│   └── c_cpp_properties.json # C++配置
├── build/                 # 构建输出目录
├── src/                   # 源代码
│   └── main.cpp          # 主程序
├── CMakeLists.txt        # CMake项目配置
├── CMakePresets.json     # CMake预设配置
└── README.md             # 项目文档
```

## 注意事项

- 确保 vcpkg 正确安装并集成到 Visual Studio
- 确保 CMake 可以找到 vcpkg 工具链文件
- 如果更改了 vcpkg 的安装位置，需要更新 CMakePresets.json 中的工具链文件路径

## 故障排除

1. 如果 CMake 找不到依赖包：

   - 检查 vcpkg 是否正确安装了所需包
   - 确认 CMakePresets.json 中的 vcpkg 工具链路径是否正确

2. 如果 VS Code 无法找到包含文件：
   - 确保安装了所需的 VS Code 扩展
   - 重新加载 VS Code 窗口
   - 删除 build 目录并重新构建项目

## 许可证

[MIT License](LICENSE)
