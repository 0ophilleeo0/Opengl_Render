#include "WindowsFuction.h"
#include "Camera.h"

namespace {
    float epsillon = 0.001;
}
void GlfwKeyWCallBack()
{
	Camera* cameraPtr = &(Camera::GetInstance());
	cameraPtr->camPosition += cameraPtr->camSpeed * cameraPtr->camFront;
	cameraPtr->status = 1;
}

void GlfwKeySCallBack()
{
	Camera* cameraPtr = &(Camera::GetInstance());
	cameraPtr->camPosition -= cameraPtr->camSpeed * cameraPtr->camFront;
	cameraPtr->status = 1;
}

void GlfwKeyACallBack()
{
	Camera* cameraPtr = &(Camera::GetInstance());
	cameraPtr->camPosition -= cameraPtr->camSpeed * glm::normalize(glm::cross(cameraPtr->camFront, cameraPtr->camUp));
	cameraPtr->status = 1;
}

void GlfwKeyDCallBack()
{
	Camera* cameraPtr = &(Camera::GetInstance());
	cameraPtr->camPosition += cameraPtr->camSpeed * glm::normalize(glm::cross(cameraPtr->camFront, cameraPtr->camUp));
	cameraPtr->status = 1;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    //w,s,a,d
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        GlfwKeySCallBack();
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        GlfwKeyWCallBack();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        GlfwKeyACallBack();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        GlfwKeyDCallBack();
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void ProcessMouseMovement(float& yaw, float& pitch, const float xoffset, const float yoffset)
{
    if (fabs(xoffset) < epsillon && fabs(yoffset) < epsillon) {
        return;
    }

    float sensitivity = 0.05;
    yaw += (xoffset * sensitivity);
    pitch += (yoffset * sensitivity);

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 postion = Camera::GetInstance().camPosition;
    Camera::GetInstance().SetPose(postion, pitch, yaw);
    Camera::GetInstance().status = 1;
}