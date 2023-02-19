#ifndef WINDOWEVENT_H
#define WINDOWEVENT_H

#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window, float deltaTime);

//the parameter is defined by glfw
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif