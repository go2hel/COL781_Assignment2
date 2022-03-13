#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

// Shader class
class Shader
{
public:
    // ID of shader
    unsigned int ID;

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="vertexShaderPath">Path to vertex shader</param>
    /// <param name="fragmentShaderPath">Path to fragment shader</param>
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
    {
        string vertexShaderCode;
        string fragmentShaderCode;
        ifstream vertexShaderFile;
        ifstream fragmentShaderFile;

        vertexShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fragmentShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

        try
        {

            vertexShaderFile.open(vertexShaderPath);
            fragmentShaderFile.open(fragmentShaderPath);

            stringstream vShaderStream, fShaderStream;
            vShaderStream << vertexShaderFile.rdbuf();
            fShaderStream << fragmentShaderFile.rdbuf();
            
            vertexShaderFile.close();
            fragmentShaderFile.close();
            
            vertexShaderCode = vShaderStream.str();
            fragmentShaderCode = fShaderStream.str();
        }
        catch (exception& e)
        {
            cout << "Cannot read shader file \n" << e.what() << endl;
        }

        const char* vertexShaderCodeC = vertexShaderCode.c_str();
        const char* fragmentShaderCodeC = fragmentShaderCode.c_str();

        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexShaderCodeC, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentShaderCodeC, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    /// <summary>
    /// Using the shader
    /// </summary>
    void use()
    {
        glUseProgram(ID);
    }

    // Setting uniforms in the given shaders

    void setFloat(const string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec3(const string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
    }

    void setMat4(const string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

private:

    /// <summary>
    /// Checks for compilation errors
    /// </summary>
    /// <param name="shader"></param>
    /// <param name="type"></param>
    void checkCompileErrors(GLuint shader, string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                cout << "Shader compilation error in " << type << "\n" << infoLog << "\n_____________________________________________________________" << endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                cout << "Shader linking error in " << type << "\n" << infoLog << "\n_____________________________________________________________" << endl;
            }
        }
    }
};
#endif