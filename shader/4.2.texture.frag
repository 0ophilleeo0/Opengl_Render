#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
//in vec3 ourColor;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float timeColor;

uniform float ambientStrength;
uniform vec3 ambientColor;

uniform vec3 objectColor;
void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//vec3 tmpColor = vec3(mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2)) * ourColor * timeColor;
	//FragColor = vec4(tmpColor, 1.0f);
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    //vec3 ambient = ambientStrength * ambientColor;

    //vec3 result = ambient * objectColor;
    FragColor = texture(texture1, TexCoord);
}