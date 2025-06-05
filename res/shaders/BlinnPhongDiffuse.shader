#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_NormalMatrix; // 用于变换法线

out vec3 FragPos;
out vec3 Normal;

void main()
{
    FragPos = vec3(u_Model * vec4(aPos, 1.0));
    Normal = vec3(u_NormalMatrix * vec4(aNormal, 0.0)); // 注意法线不进行平移
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos; // 相机位置
uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;

void main()
{
    // 环境光 (简化，这里只关注漫反射)
    // vec3 ambient = 0.1 * u_LightColor;

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor * u_ObjectColor;

    FragColor = vec4(diffuse, 1.0); // 暂时只显示漫反射
}
