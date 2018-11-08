//
// Created by xuyz on 2018/11/8.
//
#include <stdio.h>
#define GLEF_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <utils/utils.h>
#include <cmath>
#include "utils/shader.h"
//
//std::string vertex_src = read_file("src/shader/vertex.glsl");
//std::string fragment_src = read_file("src/shader/fragment.glsl");
//
//GLuint createShader(GLenum type, const GLchar* source){
////    std::cout << source;
//    GLuint shader = glCreateShader(type);
//    glShaderSource(shader, 1, &source, nullptr);
//    glCompileShader(shader);
//    GLint success;
//    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//    if (!success){
//        GLchar buffer[512];
//        glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
//        printf("shader info log:%s\n", buffer);
//        return 0;
//    }
//
//    return shader;
//}
//
//GLuint createProgram(GLuint vertex_shader, GLuint fragment_shader){
//    GLuint program = glCreateProgram();
//    glAttachShader(program, vertex_shader);
//    glAttachShader(program, fragment_shader);
//    glLinkProgram(program);
//    GLint success;
//    glGetProgramiv(program, GL_LINK_STATUS, &success);
//    if (!success){
//        GLchar buffer[512];
//        glGetProgramInfoLog(program, sizeof(buffer), nullptr, buffer);
//        printf("program info log:%s\n", buffer);
//        return 0;
//    }
//    return program;
//}

//*  @param[in] window The window that received the event.
//*  @param[in] key The [keyboard key](@ref keys) that was pressed or released.
//*  @param[in] scancode The system-specific scancode of the key.
//*  @param[in] action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.
//*  @param[in] mods Bit field describing which [modifier keys](@ref mods) were
//        *  held down.

void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mods){
    if (key == GLFW_KEY_ESCAPE && action==GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(400, 400, "hello-shader", nullptr, nullptr);
    if (window == nullptr){
        const GLchar * error;
        glfwGetError(&error);
        printf("%s", error);
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GLFW_TRUE;
    glewInit();

//    GLint nrAttributes;
//    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
//    printf("max vertex attributes:%d\n", nrAttributes);

    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


//    GLuint vs = createShader(GL_VERTEX_SHADER, vertex_src.c_str());
//    GLuint fs = createShader(GL_FRAGMENT_SHADER, fragment_src.c_str());
//    GLuint program = createProgram(vs, fs);
//    glDeleteShader(vs);
//    glDeleteShader(fs);

    GLfloat vertexes[] = {
            //位置             //颜色
            0,    0.5,  0,    1.0, 0, 0,
            0.5,  -0.5f, 0,    0, 1.0, 0,
            -0.5f, -0.5f, 0,    0, 0, 1.0,
    };

    GLuint indices[] = {0, 1, 2};


    GLuint VAO, VBO, VEO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VEO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), ((GLfloat*)0)+3);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    Shader shader("src/shader/vertex.glsl", "src/shader/fragment.glsl");
//    glUseProgram(program);
    shader.Use();

    glBindVertexArray(0);



    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        GLdouble timeVal = glfwGetTime();
        GLfloat redVal = sin(timeVal)/2 + 0.5;
        GLint location = glGetUniformLocation(shader.Program, "ourColor");
        glUniform4f(location, redVal, 0, 0, 1.0);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}