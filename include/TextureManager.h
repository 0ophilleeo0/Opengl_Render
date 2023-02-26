#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <glad/glad.h>
#include <unordered_map>
#include <string>

enum class ColorType {
	RGB,
	RGBA
};

class TextureManager
{
public:
	TextureManager(const std::string& path) :basePath(path) {}
	bool LoadTexture(std::string textureName, ColorType type, bool flip = false);
	GLuint GetTextureID(std::string textureName);
private:
	std::string basePath;
	std::unordered_map<std::string, GLuint> textureIDMap;
};
#endif