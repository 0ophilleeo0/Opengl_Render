#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 worldPos;
    vec3 worldNormal;
    vec2 texCoord;
    vec4 fragLightPos;
} fs_in;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform sampler2D diffuseSampler;
uniform sampler2D shadowSampler;

float CalShadow(vec4 fragLightSpace)
{
    vec3 projCoord = fragLightSpace.xyz / fragLightSpace.w;
    projCoord = projCoord * 0.5 + 0.5;
    float depthMap = texture(shadowSampler, projCoord.xy).r;
    float fragDepth = projCoord.z;
    float bias = 0.005;
    float shadow = (fragDepth - bias > depthMap) ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    // ambient
    float ambient = 0.1;
  	
    // diffuse 
    vec3 norm = normalize(fs_in.worldNormal);
    vec3 lightDir = normalize(lightPos - fs_in.worldPos);
    float diffuse = max(dot(norm, lightDir), 0.0);
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fs_in.worldNormal);
    vec3 reflectDir = reflect(-lightDir, norm); 
    vec3 halfV = normalize(viewDir + lightDir);
    float specular = pow(max(dot(halfV, norm), 0.0), 32) * specularStrength;
    
    //shadow
    float shadow = CalShadow(fs_in.fragLightPos);

    //vec4 texColor = mix(texture(txSampler0, TexCoord), texture(txSampler1, TexCoord), 0.2);
    vec4 texColor = texture(diffuseSampler, fs_in.texCoord);
    vec3 objecColor = vec3(texColor);

    vec3 light = (ambient + (1 - shadow) * (diffuse + specular)) * lightColor;
    vec3 result = light * objecColor;
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(vec3(shadow), 1.0);
} 