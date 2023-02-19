#ifndef OS_H
#define OS_H

#include <GLFW/glfw3.h>
#include "ErrorCode.h"
#include <functional>

class OS
{
public:
    OS(unsigned int w, unsigned int h) :width(w), height(h) {}
    RetCode Initialize();
    GLFWwindow* GetWindow() { return window; }
private:
    void InitializeAndConfigure();
    void SetWindowFuction();

    RetCode GlfwWindowCreation();
    RetCode LoadOpenGLFunction();

    unsigned int width;
    unsigned int height;

    GLFWwindow* window = nullptr;
};
#endif