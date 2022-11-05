#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
//out vec3 ourColor;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	//vec4 world = vec4(aPos, 1.0);
	//vec4 tmp = vec4(0, 0, 0, 0);
	//world = world + tmp;
	//gl_Position = Projection * world;
	//gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	//ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}