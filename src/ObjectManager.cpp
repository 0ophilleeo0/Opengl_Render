#include "ObjectManager.h"
#include "vertices.h"

void Object::CreatMesh2DVAO()
{
    glGenVertexArrays(1, &mesh2DVAO);
    glBindVertexArray(mesh2DVAO);

    unsigned int mesh2DVBO;
    glGenBuffers(1, &mesh2DVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh2DVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh2DVertices), mesh2DVertices, GL_STATIC_DRAW);
    vboTypeIDMap[ObjectType::mesh2D] = mesh2DVBO;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
}

void Object::CreatMesh3DVAO()
{
    glGenVertexArrays(1, &mesh3DVAO);
    glBindVertexArray(mesh3DVAO);

    unsigned int mesh3DVBO;
    glGenBuffers(1, &mesh3DVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh3DVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh3DVertices), mesh3DVertices, GL_STATIC_DRAW);
    vboTypeIDMap[ObjectType::mesh3D] = mesh3DVBO;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glBindVertexArray(0);
}

void Object::CreatPlaneVAO()
{
    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);

    unsigned int planeVBO;
    glGenBuffers(1, &planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    vboTypeIDMap[ObjectType::plane] = planeVBO;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glBindVertexArray(0);
}

void Object::CreatCubeVAO()
{
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    unsigned int cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    vboTypeIDMap[ObjectType::cube] = cubeVBO;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glBindVertexArray(0);
}

Object::~Object()
{
    // optional: de-allocate all resources once they've outlived their purpose:
    if (cubeVAO != 0) {
        glDeleteVertexArrays(1, &cubeVAO);
    }
    
    if (mesh2DVAO != 0) {
        glDeleteVertexArrays(1, &mesh2DVAO);
    }

    if (mesh3DVAO != 0) {
        glDeleteVertexArrays(1, &mesh3DVAO);
    }

    if (planeVAO != 0) {
        glDeleteVertexArrays(1, &planeVAO);
    }

    for (auto& vbo : vboTypeIDMap) {
        glDeleteBuffers(1, &vbo.second);
    }
}