#ifndef WINDWOSFUCTION_H
#define WINDWOSFUCTION_H

#include <GLFW/glfw3.h>
void GlfwKeyWCallBack();
void GlfwKeySCallBack();
void GlfwKeyACallBack();
void GlfwKeyDCallBack();

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessMouseMovement(float& yaw, float& pitch, const float xoffset, const float yoffset);
#endif