#ifndef OS_H
#define OS_H

#include <GLFW/glfw3.h>
#include "ErrorCode.h"
#include <functional>

class OS
{
public:
    OS(unsigned int w, unsigned int h) :width(w), height(h) {}
    ErrorCode Initialize();
    GLFWwindow* GetWindow() { return window; }
private:
    void InitializeAndConfigure();
    void SetWindowFuction();

    ErrorCode GlfwWindowCreation();
    ErrorCode LoadOpenGLFunction();

    unsigned int width;
    unsigned int height;

    GLFWwindow* window = nullptr;
};
#endif