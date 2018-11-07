//
// Created by xuyz on 2018/11/7.
//
#include <stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//*  @param[in] window The window that received the event.
//*  @param[in] key The [keyboard key](@ref keys) that was pressed or released.
//*  @param[in] scancode The system-specific scancode of the key.
//*  @param[in] action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.
//*  @param[in] mods Bit field describing which [modifier keys](@ref mods) were
//        *  held down.
void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods);

GLint Width = 600, Height = 400;

const GLchar *vertexShaderSource =
        "#version 330 core\n"
        "layout (location 0) in vec3 position;\n"
        "void main(){\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}"
        ;
const GLchar *fragmentShaderSource =
        "#version 330 core\n"
        ""

int main(void){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(Width, Height, "hello-triangle", nullptr, nullptr);
    if (window== nullptr){
        const char *str;
        glfwGetError(&str);
        printf("glfwCreateWindow failed:%s\n", str);
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK!= err){
        printf("glew init error:%s\n", glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }

    GLint width, height;
    glfwGetFramebufferSize(window,&width, &height);
    glViewport(0, 0, width, height);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.2f,0.3f,0.3f,1);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods){
    if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}