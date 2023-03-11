#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <glad/glad.h>
#include <glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

class Shader
{
public:
    void Init(const std::string& path){
        basePath = path;
    }

    void CompileShader(std::string vertexPath, std::string fragmentPath, const std::string name);

    // activate the shader
    // ------------------------------------------------------------------------
    void use(const std::string& name)
    {
        if (shaderIDMap.count(name) == 0) {
            std::cout << "Shader use : Illegal name!" << std::endl;
            return;
        }

        GLuint ID = shaderIDMap[name];
        glUseProgram(ID);
    }

    GLuint GetID(const std::string& name)
    {
        if (shaderIDMap.count(name) == 0) {
            std::cout << "Shader GetID : Illegal name!" << std::endl;
            return 0;
        }
        else {
            return shaderIDMap[name];
        }
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& shaderName, const std::string& name, bool value)
    {
        GLuint ID = shaderIDMap[shaderName];
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& shaderName, const std::string& name, int value)
    {
        GLuint ID = shaderIDMap[shaderName];
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& shaderName, const std::string& name, float value)
    {
        GLuint ID = shaderIDMap[shaderName];
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& shaderName, const std::string& name, const glm::vec2& value)
    {
        GLuint ID = shaderIDMap[shaderName];
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& shaderName, const std::string& name, float x, float y)
    {
        GLuint ID = shaderIDMap[shaderName];
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& shaderName, const std::string& name, const glm::vec3& value)
    {
        GLuint ID = shaderIDMap[shaderName];
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& shaderName, const std::string& name, float x, float y, float z)
    {
        GLuint ID = shaderIDMap[shaderName];
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& shaderName, const std::string& name, const glm::vec4& value)
    {
        GLuint ID = shaderIDMap[shaderName];
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& shaderName, const std::string& name, float x, float y, float z, float w)
    {
        GLuint ID = shaderIDMap[shaderName];
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& shaderName, const std::string& name, const glm::mat2& mat)
    {
        GLuint ID = shaderIDMap[shaderName];
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& shaderName, std::string name, const glm::mat3& mat)
    {
        GLuint ID = shaderIDMap[shaderName];
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& shaderName, std::string name, const glm::mat4& mat)
    {
        GLuint ID = shaderIDMap[shaderName];
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void CheckCompileErrors(GLuint shader, std::string type);
    void LoadShaderResource(const char* vertexPath, const char* fragmentPath);

    std::string vertexCode;
    std::string fragmentCode;
    std::string basePath;
    std::unordered_map<std::string, GLuint> shaderIDMap;
};
#endif