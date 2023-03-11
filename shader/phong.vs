#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;

out VS_OUT
{
    vec3 worldPos;
    vec3 worldNormal;
    vec2 texCoord;
    vec4 fragLightPos;
} vs_out;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    vs_out.worldPos = vec3(model * vec4(aPos, 1.0));
    vs_out.worldNormal = mat3(transpose(inverse(model))) * aNormal;  
    vs_out.texCoord = aTexture;
    vs_out.fragLightPos = lightSpaceMatrix * vec4(vs_out.worldPos, 1.0);
    gl_Position = projection * view * vec4(vs_out.worldPos, 1.0);
}