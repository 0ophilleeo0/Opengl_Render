#include "OS.h"
#include "WindowEvent.h"
#include <iostream>
#include <glad/glad.h>


RetCode OS::Initialize()
{
    RetCode res = RetCode::SUCCESS;
    InitializeAndConfigure();
    res = GlfwWindowCreation();
    SetWindowFuction();
    res = LoadOpenGLFunction();
    return res;
}

void OS::InitializeAndConfigure()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
  
RetCode OS::GlfwWindowCreation()
{
    // glfw window creation
    // --------------------
    window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return RetCode::FAILED;
    }
    glfwMakeContextCurrent(window);

    return RetCode::SUCCESS;
}

void OS::SetWindowFuction()
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

RetCode OS::LoadOpenGLFunction()
{
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return RetCode::FAILED;
    }

    return RetCode::SUCCESS;
}
