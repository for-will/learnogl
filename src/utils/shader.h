//
// Created by puffer on 2018/11/8.
//

#ifndef UTILS_SHADER_H
#define UTILS_SHADER_H

#include <GL/glew.h>
#include "utils/utils.h"
class Shader{
public:
    GLuint Program;

    Shader(const GLchar* vertexShaderSourcePath, const GLchar* fragmentShaderSourcePath){
        std::string vSource = read_file(vertexShaderSourcePath);
        std::string fSource = read_file(fragmentShaderSourcePath);
        GLuint vs = createShader(GL_VERTEX_SHADER, vSource.c_str());
        GLuint fs = createShader(GL_FRAGMENT_SHADER, fSource.c_str());
        Program = createProgram(vs, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    ~Shader(){
        glDeleteProgram(Program);
        Program = 0;
    }

    void Use(){
        glUseProgram(Program);
    }

private:

    GLuint createShader(GLenum type, const GLchar* source){
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success){
            GLchar buffer[512];
            glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
            printf("%s::(%s) ERROR:\n%s\n", __FUNCTION__, source, buffer);
            return 0;
        }

        return shader;
    }

    GLuint createProgram(GLuint vertex_shader, GLuint fragment_shader){
        GLuint program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success){
            GLchar buffer[512];
            glGetProgramInfoLog(program, sizeof(buffer), nullptr, buffer);
            printf("%s::ERROR:\n%s\n",__FUNCTION__, buffer);
            return 0;
        }
        return program;
    }
};

#endif //UTILS_SHADER_H
