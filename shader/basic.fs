#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform bool useTex;
uniform vec4 baseColor;
uniform sampler2D texSample;

void main()
{
    if (useTex) {
        FragColor = texture(texSample, TexCoord);
    } else {
        FragColor = baseColor;
    }
    
}