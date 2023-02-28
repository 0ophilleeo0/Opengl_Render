#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 posi = projection * view * vec4(aPos, 1.0);
	gl_Position = posi.xyww;
	vec3 direction = vec3(aPos.x, -aPos.y, aPos.z);
	TexCoord = direction;
}