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

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// lighting
glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

void RenderOpaquePass(const std::string& opaque, GLuint depthMap, glm::mat4 lightSpaceMatrix);
//#define TRANSPARENT
//#define LIGHTCUBE
//#define DEPTH_DEBUG_VIEW
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
    Shader& shaderManager = Singleton<Shader>::GetInstacnce();
    shaderManager.Init("E:/github/Opengl_Render/shader/");
    shaderManager.CompileShader("phong.vs", "phong.fs", "phong");
    shaderManager.CompileShader("basic.vs", "basic.fs", "basic");
    shaderManager.CompileShader("textureFbo.vs", "textureFbo.fs", "texFBOShader");
    shaderManager.CompileShader("SkyBox.vs", "SkyBox.fs", "SkyBoxShader");
    shaderManager.CompileShader("simpleDepth.vs", "simpleDepth.fs", "simpleDepth");

    // Load texture
    // ------------------------------------
    TextureManager& textureManager = Singleton<TextureManager>::GetInstacnce();
    textureManager.Init("E:/github/Opengl_Render/texture/");
    textureManager.LoadTexture("container.jpg", ColorType::RGB);
    textureManager.LoadTexture("container2.png", ColorType::RGBA);
    textureManager.LoadTexture("blending_transparent_window.png", ColorType::RGBA);
    textureManager.LoadTexture("awesomeface.png", ColorType::RGBA, true);


    std::vector<std::string> faces
    {
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/bottom.jpg",
        "skybox/top.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
    };

    textureManager.LoadCubeTexture("skyCubeMap", faces, ColorType::RGB);
    // Load scene geometry
    // ------------------------------------
    Object& objectManager = Singleton<Object>::GetInstacnce();
    objectManager.CreatCubeVAO();
    objectManager.CreatMesh2DVAO();
    objectManager.CreatMesh3DVAO();
    objectManager.CreatPlaneVAO();

    std::vector<glm::vec3> veg_pos;
    veg_pos.push_back(glm::vec3(-1.5f, 0.5f, -0.48f));
    veg_pos.push_back(glm::vec3(1.5f, 0.5f, 0.51f));
    veg_pos.push_back(glm::vec3(0.0f, 0.5f, 0.7f));
    veg_pos.push_back(glm::vec3(-0.3f, 0.5f, -2.3f));
    veg_pos.push_back(glm::vec3(0.5f, 0.5f, -0.6f));

    
    unsigned int blockIndex = 0;
    unsigned int basicIndex = glGetUniformBlockIndex(shaderManager.GetID("basic"), "Matrices");
    unsigned int phongIndex = glGetUniformBlockIndex(shaderManager.GetID("phong"), "Matrices");
    glUniformBlockBinding(shaderManager.GetID("basic"), basicIndex, blockIndex);
    glUniformBlockBinding(shaderManager.GetID("phong"), phongIndex, blockIndex);

    //ubo
    GLuint uboMatrix;
    glGenBuffers(1, &uboMatrix);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrix);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uboMatrix);

    // projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(cameraPtr->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


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
#ifdef OFFSCREEN
        // render
        if (msaaNum > 0) {
            glBindFramebuffer(GL_FRAMEBUFFER, viewport.multiSampleFBO);
        }
        else {
            glBindFramebuffer(GL_FRAMEBUFFER, viewport.intermediateFBO);
        }
#endif
        // view transformations
        // set the view and projection matrix in the uniform block - we only have to do this once per loop iteration.
        glm::mat4 view = cameraPtr->GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrix);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glm::mat4 model = glm::mat4(1.0f);
        
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        GLfloat near_plane = 0.01f, far_plane = 20.0f;
        lightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, near_plane, far_plane);
        //lightProjection = glm::perspective(45.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // Note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene.
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // - now render scene from light's point of view
        //shaderManager.use("simpleDepth");
        //shaderManager.setMat4("simpleDepth", "lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        RenderOpaquePass("simpleDepth", depthMap, lightSpaceMatrix);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
#ifdef DEPTH_DEBUG_VIEW
        //render Depth map to quad for visual debugging
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderManager.use("texFBOShader");
        glBindVertexArray(objectManager.GetMesh2DVAO());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        shaderManager.setInt("texFBOShader", "screenTexture", 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
#endif  
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 我们现在不使用模板缓冲
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        RenderOpaquePass("phong", depthMap, lightSpaceMatrix);
        
#ifdef LIGHTCUBE
        // render lightcube
        glEnable(GL_CULL_FACE);
        shaderManager.use("basic");
        shaderManager.setBool("basic", "useTex", false);
        shaderManager.setVec4("basic", "baseColor", glm::vec4(0.5, 0.5, 0.5, 1.0));

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.5f)); // a smaller cube
        shaderManager.setMat4("basic", "model", model);

        glBindVertexArray(objectManager.GetCubeVAO());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDisable(GL_CULL_FACE);
#endif

#ifdef SKYBOX
        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        shaderManager.use("SkyBoxShader");
        glm::mat4 skyView = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
        shaderManager.setMat4("SkyBoxShader", "view", skyView);
        shaderManager.setMat4("SkyBoxShader", "projection", projection);

        glBindVertexArray(objectManager.GetCubeVAO());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureManager.GetTextureID("skyCubeMap"));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
#endif

#ifdef TRANSPARENT
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
        glBindVertexArray(objectManager.GetMesh3DVAO());

        shaderManager.use("basic");
        shaderManager.setBool("basic", "useTex", true);
        shaderManager.setInt("basic", "txSampler0", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureManager.GetTextureID("blending_transparent_window.png"));

        for (auto& pos : veg_pos)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            shaderManager.setMat4("basic", "model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
#endif // TRANSPARENT

#ifdef MSAA
        // Blit Frame
        if (msaaNum > 0) {
            // 将多重采样缓冲还原到中介FBO上
            glBindFramebuffer(GL_READ_FRAMEBUFFER, viewport.multiSampleFBO);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, viewport.intermediateFBO);
            glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }
#endif

#ifdef OFFSCREEN
        //off_screen rendering
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderManager.use("texFBOShader");
        glBindVertexArray(objectManager.GetMesh2DVAO());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, viewport.colorBuffer);	// use the color attachment texture as the texture of the quad planes
        shaderManager.setInt("texFBOShader", "screenTexture", 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
#endif
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

void RenderOpaquePass(const std::string& opaque, GLuint depthMap, glm::mat4 lightSpaceMatrix)
{
    Shader& shaderManager = Singleton<Shader>::GetInstacnce();
    TextureManager& textureManager = Singleton<TextureManager>::GetInstacnce();
    Viewport& viewport = Singleton<Viewport>::GetInstacnce();
    Object& objectManager = Singleton<Object>::GetInstacnce();

    // render cube
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBindVertexArray(objectManager.GetCubeVAO());
    shaderManager.use(opaque);

    //set texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureManager.GetTextureID("container.jpg"));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    shaderManager.setInt(opaque, "diffuseSampler", 0);
    shaderManager.setInt(opaque, "shadowSampler", 1);

    shaderManager.setVec3(opaque, "lightColor", 1.0f, 1.0f, 1.0f);
    shaderManager.setVec3(opaque, "lightPos", lightPos);
    shaderManager.setVec3(opaque, "viewPos", viewport.cameraPtr->Position);

    //transformation
    glm::mat4 model = glm::mat4(1.0f);
    shaderManager.setMat4(opaque, "model", model);
    shaderManager.setMat4(opaque, "lightSpaceMatrix", lightSpaceMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    //render floot
    glDisable(GL_CULL_FACE);
    glBindVertexArray(objectManager.GetPlaneVAO());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureManager.GetTextureID("container2.png"));

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    shaderManager.setMat4(opaque, "model", model);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}