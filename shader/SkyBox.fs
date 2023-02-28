#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube cubeSampler;

void main()
{
	FragColor = texture(cubeSampler, TexCoord);
}