#include "ShaderClass.h"
#include "vertices.h"
#include "OS.h"
#include "Viewport.h"
#include "WindowEvent.h"
#include "Singleton.h"

#include <GLFW/glfw3.h>
#include <string>

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

    Viewport& viewport = Singleton<Viewport>::GetInstacnce();
    std::shared_ptr<Camera> cameraPtr = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    viewport.Init(SCR_WIDTH, SCR_HEIGHT, cameraPtr, 0);

    // build and compile our shader zprogram
    // ------------------------------------
    std::shared_ptr<Shader> shaderManager = std::make_shared<Shader>("E:/github/Opengl_Render/shader/");
    shaderManager->CompileShader("2.2_basic_light.vs", "2.2_basic_light.fs", "basic_light");
    shaderManager->CompileShader("2.2_light_cube.vs", "2.2_light_cube.fs", "light_cube");
    shaderManager->CompileShader("textureFbo.vs", "textureFbo.fs", "texFBOShader");

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

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
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        shaderManager->use("basic_light");
        shaderManager->setVec3("basic_light", "objectColor", 1.0f, 0.5f, 0.31f);
        shaderManager->setVec3("basic_light", "lightColor", 1.0f, 1.0f, 1.0f);
        shaderManager->setVec3("basic_light", "lightPos", lightPos);
        shaderManager->setVec3("basic_light", "viewPos", cameraPtr->Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(cameraPtr->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = cameraPtr->GetViewMatrix();
        shaderManager->setMat4("basic_light", "projection", projection);
        shaderManager->setMat4("basic_light", "view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        shaderManager->setMat4("basic_light", "model", model);

        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // also draw the lamp object
        shaderManager->use("light_cube");
        shaderManager->setMat4("light_cube", "projection", projection);
        shaderManager->setMat4("light_cube", "view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        shaderManager->setMat4("light_cube", "model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(os.GetWindow());
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    viewport.Free();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}