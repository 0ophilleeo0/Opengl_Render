#include "Singleton.h"
#include "WindowEvent.h"
#include "OS.h"
#include "Viewport.h"

#include "ShaderManager.h"
#include "TextureManager.h"
#include "ObjectManager.h"

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <algorithm>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

int main()
{
    OS os(SCR_WIDTH, SCR_HEIGHT);
    os.Initialize();

    int msaaNum = 4;
    Viewport& viewport = Singleton<Viewport>::GetInstacnce();
    std::shared_ptr<Camera> cameraPtr = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    viewport.Init(SCR_WIDTH, SCR_HEIGHT, cameraPtr, msaaNum);

    // build and compile our shader zprogram
    // ------------------------------------
    std::shared_ptr<Shader> shaderManager = std::make_shared<Shader>("E:/github/Opengl_Render/shader/");
    shaderManager->CompileShader("phong.vs", "phong.fs", "phong");
    shaderManager->CompileShader("basic.vs", "basic.fs", "basic");
    shaderManager->CompileShader("textureFbo.vs", "textureFbo.fs", "texFBOShader");

    // Load texture
    // ------------------------------------
    std::shared_ptr<TextureManager> textureManager = std::make_shared<TextureManager>("E:/github/Opengl_Render/texture/");
    textureManager->LoadTexture("container.jpg", ColorType::RGB);
    textureManager->LoadTexture("container2.png", ColorType::RGB);
    textureManager->LoadTexture("blending_transparent_window.png", ColorType::RGBA);
    textureManager->LoadTexture("awesomeface.png", ColorType::RGBA, true);

    // Load scene geometry
    // ------------------------------------
    Object& objectManager = Singleton<Object>::GetInstacnce();
    objectManager.CreatCubeVAO();
    objectManager.CreatMesh2DVAO();
    objectManager.CreatMesh3DVAO();

    std::vector<glm::vec3> veg_pos;
    veg_pos.push_back(glm::vec3(-1.5f, 0.5f, -0.48f));
    veg_pos.push_back(glm::vec3(1.5f, 0.5f, 0.51f));
    veg_pos.push_back(glm::vec3(0.0f, 0.5f, 0.7f));
    veg_pos.push_back(glm::vec3(-0.3f, 0.5f, -2.3f));
    veg_pos.push_back(glm::vec3(0.5f, 0.5f, -0.6f));

    // render loop
    // -----------
    while (!glfwWindowShouldClose(os.GetWindow()))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(os.GetWindow(), deltaTime);

        // render
        if (msaaNum > 0) {
            glBindFramebuffer(GL_FRAMEBUFFER, viewport.multiSampleFBO);
        }
        else {
            glBindFramebuffer(GL_FRAMEBUFFER, viewport.intermediateFBO);
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �������ڲ�ʹ��ģ�建��
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        // render cube
        shaderManager->use("phong");
        shaderManager->setVec3("phong", "lightColor", 1.0f, 1.0f, 1.0f);
        shaderManager->setVec3("phong", "lightPos", lightPos);
        shaderManager->setVec3("phong", "viewPos", cameraPtr->Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(cameraPtr->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = cameraPtr->GetViewMatrix();
        shaderManager->setMat4("phong", "projection", projection);
        shaderManager->setMat4("phong", "view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        shaderManager->setMat4("phong", "model", model);

        //set texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureManager->GetTextureID("container.jpg"));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureManager->GetTextureID("awesomeface.png"));

        shaderManager->setInt("phong", "txSampler0", 0);
        shaderManager->setInt("phong", "txSampler1", 1);

        glBindVertexArray(objectManager.GetCubeVAO());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // render lightcube
        shaderManager->use("basic");
        shaderManager->setBool("basic", "useTex", false);
        shaderManager->setVec4("basic", "baseColor", glm::vec4(1.0, 1.0, 1.0, 0.0));

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        shaderManager->setMat4("basic", "model", model);
        shaderManager->setMat4("basic", "view", view);
        shaderManager->setMat4("basic", "projection", projection);

        glBindVertexArray(objectManager.GetCubeVAO());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDisable(GL_CULL_FACE);

        //transparent sort
        //sort
        std::sort(veg_pos.begin(), veg_pos.end(), [&cameraPtr](glm::vec3 a, glm::vec3 b)
            {
                float distanceA = glm::length(cameraPtr->Position - a);
                float distanceB = glm::length(cameraPtr->Position - b);
                return distanceA > distanceB;
            });

        //transparent render
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shaderManager->setBool("basic", "useTex", true);
        shaderManager->setInt("basic", "txSampler0", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureManager->GetTextureID("blending_transparent_window.png"));
        glBindVertexArray(objectManager.GetMesh3DVAO());

        for (auto& pos : veg_pos)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            shaderManager->setMat4("basic", "model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // Blit Frame
        if (msaaNum > 0) {
            // �����ز������廹ԭ���н�FBO��
            glBindFramebuffer(GL_READ_FRAMEBUFFER, viewport.multiSampleFBO);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, viewport.intermediateFBO);
            glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }

        //off_screen rendering
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderManager->use("texFBOShader");
        glBindVertexArray(objectManager.GetMesh2DVAO());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, viewport.colorBuffer);	// use the color attachment texture as the texture of the quad planes
        shaderManager->setInt("texFBOShader", "screenTexture", 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(os.GetWindow());
        glfwPollEvents();
    }

    viewport.Free();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}