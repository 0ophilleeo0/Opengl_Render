#include "ShaderManager.h"

void Shader::LoadShareFile(const char* path, std::string& shadeCode)
{
    // 1. retrieve the source code from filePath
    std::ifstream shaderFile;

    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shadeCode = shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }
}

void Shader::CheckCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::CompileShader(const std::string& name, std::string vertexPath, std::string fragmentPath, \
    std::string geometryPath)
{
    auto LoadCompile = [this](std::string path, unsigned int& shaderID, std::string type) {
        //1. load shade code
        path = this->basePath + path;
        std::string shadeCodeStr;
        LoadShareFile(path.c_str(), shadeCodeStr);
        const char* shadeCode = shadeCodeStr.c_str();

        // 2. compile shaders
        if (type == "VERTEX") {
            shaderID = glCreateShader(GL_VERTEX_SHADER);
        }
        else if (type == "FRAGMENT") {
            shaderID = glCreateShader(GL_FRAGMENT_SHADER);
        }
        else {
            shaderID = glCreateShader(GL_GEOMETRY_SHADER);
        }
        glShaderSource(shaderID, 1, &shadeCode, NULL);
        glCompileShader(shaderID);
        CheckCompileErrors(shaderID, type);
    };

    unsigned int vertex = 0;
    unsigned int fragment = 0;
    unsigned int geometry = 0;

    LoadCompile(vertexPath, vertex, "VERTEX");
    LoadCompile(fragmentPath, fragment, "FRAGMENT");
    if (geometryPath != "") {
        LoadCompile(geometryPath, geometry, "GEOMETRY");
    }

    // shader Program
    GLuint ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if (geometryPath != "") {
        glAttachShader(ID, geometry);
    }
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");
    shaderIDMap[name] = ID;

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != "") {
        glDeleteShader(geometry);
    }
}



