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
	void Init(const std::string& path) {
		basePath = path;
	}

	bool LoadTexture(std::string textureName, ColorType type, bool flip = false);
	bool LoadCubeTexture(std::string cubeName, const std::vector<std::string>& faces,\
		ColorType type, bool flip = false);
	GLuint GetTextureID(std::string textureName);
private:
	std::string basePath;
	std::unordered_map<std::string, GLuint> textureIDMap;
};
#endif