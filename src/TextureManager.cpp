#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

bool TextureManager::LoadTexture(std::string textureName, ColorType type, bool flip)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载并生成纹理
    int width, height, nrChannels;
    std::string picName = basePath + textureName;

    if (flip) {
        stbi_set_flip_vertically_on_load(true);
    }

    unsigned char* data = stbi_load(picName.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        if (type == ColorType::RGB) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        
        if (type == ColorType::RGBA) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);
        textureIDMap[textureName] = textureID;
    }
    else
    {
        std::cout << "Failed to load texture:" << picName << std::endl;
        return false;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

bool TextureManager::LoadCubeTexture(std::string cubeName, const std::vector<std::string>& faces,\
    ColorType type, bool flip)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // 加载并生成纹理
    int width, height, nrChannels;
    unsigned char* data;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if (flip) {
        stbi_set_flip_vertically_on_load(true);
    }

    for (int i = 0; i < 6; ++i) {
        std::string picPath = basePath + faces[i];
        unsigned char* data = stbi_load(picPath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    textureIDMap[cubeName] = textureID;
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return true;
}

GLuint TextureManager::GetTextureID(std::string textureName)
{
    if (textureIDMap.count(textureName) == 0) {
        std::cout << "TextureManager: textureName error!" << std::endl;
        return 0;
    }
    return textureIDMap[textureName];
    
}