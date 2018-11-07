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
        "layout (location = 0) in vec3 position;\n"
        "void main(){\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1);\n"
        "}"
        ;
const GLchar *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main(){\n"
        "color=vec4(1.0, 0.5, 0.2, 1.0);\n"
        "}"
        ;

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

    GLint success;
    GLchar info[512];
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, sizeof(info), nullptr, info);
        printf("GL_VERTEX_SHADER shader info log:%s\n", info);
        return -1;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, sizeof(info), nullptr, info);
        printf("GL_FRAGMENT_SHADER shader info log:%s\n", info);
        return -1;
    }

    // 着色器程序
    GLuint shaderProgram=glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(shaderProgram, sizeof(info), nullptr, info);
        printf("GL_LINK_STATUS: %s\n", info);
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //vertexes
    GLfloat vertexes[] ={
            -0.5,-0.5,1.0,
            0.5,-0.5,1.0,
            0,0.5,0.9
    };

    // VAO{VBO}
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.2f,0.3f,0.3f,1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods){
    if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}