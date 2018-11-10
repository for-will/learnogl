//
// Created by puffer on 2018/11/9.
//

//
// Created by xuyz on 2018/11/8.
//
#include <stdio.h>
#define GLEF_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <string>
#include <utils/utils.h>
#include <cmath>
#include "utils/shader.h"

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

    GLFWwindow *window = glfwCreateWindow(600, 600, "test-texture", nullptr, nullptr);
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


    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);



    GLfloat vertexes[] = {
            //位置                 //颜色            //纹理
            0.5,    0.5,    0,     1.0, 0,   0,     1,   1,
            -0.5f,  0.5f,   0,     0,   1.0, 0,     0,   1,
            -0.5f,  -0.5f,  0,     0,   0,   1.0,   0,   0,
            0.5,    -0.5f,  0,     0,   1.0, 0,     1,   0,
    };

    GLuint indices[] = {0, 1, 2, 0, 2, 3};


    GLuint VAO, VBO, VEO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VEO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), ((GLfloat*)0)+3);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8* sizeof(GLfloat), ((GLfloat*)0)+6);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    Shader shader("src/shader/vertex.glsl", "src/shader/fragment.glsl");
    shader.Use();

    glBindVertexArray(0);


    // load texture
    GLint textureWidth, textureHeight;
    GLubyte * image1 = SOIL_load_image("texture/container.jpg", &textureWidth, &textureHeight, nullptr, SOIL_LOAD_RGB);

//    printf("textureWidth:%d, textureHeight:%d\n\n", textureWidth, textureHeight);


    GLuint Textures[2];
    glGenTextures(2, Textures);

    glBindTexture(GL_TEXTURE_2D, Textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
    glGenerateMipmap(GL_TEXTURE_2D);


    GLubyte * image2 = SOIL_load_image("texture/awesomeface.png", &textureWidth, &textureHeight, nullptr, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, Textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image1);
    SOIL_free_image_data(image2);

    glBindTexture(GL_TEXTURE_2D, 0);




    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);



        glBindVertexArray(VAO);

//        GLdouble timeVal = glfwGetTime();
//        GLfloat redVal = sin(timeVal)/2 + 0.5;
//        GLint location = glGetUniformLocation(shader.Program, "ourColor");
//        glUniform4f(location, redVal, 0, 0, 1.0);


        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, Textures[0]);
        glUniform1i(glGetUniformLocation(shader.Program, "ourTexture1"), 0);


        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Textures[1]);

        glUniform1i(glGetUniformLocation(shader.Program, "ourTexture2"), 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VEO);


    glfwTerminate();
    return 0;
}