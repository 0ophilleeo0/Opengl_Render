#ifndef VIEWPORT_H
#define VIEWPORT_H
#include "Camera.h"
#include "ErrorCode.h"
#include <memory>
class Viewport
{
public:
	Viewport() = default;
	~Viewport() = default;

	RetCode Init(const int w, const int h, std::shared_ptr<Camera>& camera, const int sample = 0);
	void Free();

	std::shared_ptr<Camera> cameraPtr = nullptr;

	GLuint intermediateFBO;
	GLuint colorBuffer;
	GLuint multiSampleFBO;
private:
	unsigned int width = 0;
	unsigned int height = 0;
};

#endif