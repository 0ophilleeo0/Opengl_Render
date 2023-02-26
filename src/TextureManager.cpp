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

GLuint TextureManager::GetTextureID(std::string textureName)
{
    if (textureIDMap.count(textureName) != 0) {
        return textureIDMap[textureName];
    }
    
    return 0;
}