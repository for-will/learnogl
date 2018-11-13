//
// Created by puffer on 2018/11/13.
//

#define GFEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string.h>
#include <SOIL.h>

#include "utils/shader.h"

static const glm::mat4 IdentityMat4({
                                            1,0,0,0,
                                            0,1,0,0,
                                            0,0,1,0,
                                            0,0,0,1,
                                    });

void dumpMat4(const glm::mat4& m){
    for (int i = 0; i < 16; ++i) {
        printf("%.2f%s", m[i%4][i/4], i%4!=3 ? " ":"\n");
    }
}

int main(void){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow * window = glfwCreateWindow(800, 600, "test trans", nullptr, nullptr);
    glfwMakeContextCurrent(window);


    glewExperimental = GL_TRUE;
    glewInit();

    GLint screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);

    GLfloat vertexes[] = {
             0.5f,  0.5f, 0, 1, 1,
            -0.5f,  0.5f, 0,  0, 1,

            -0.5f, -0.5f, 0,  0, 0,
             0.5f, -0.5f, 0,  1, 0,

             0.5f,  0.5f,  -1,  1, 1,
             -0.5f,  0.5f, -1,  0, 1,

             -0.5f, -0.5f, -1,  0, 0,
             0.5f, -0.5f,  -1,  1, 0,
    };

    GLuint indices[] = {
            //前
            0, 1, 2,
            0, 2, 3,
            // 后
            4, 5, 6,
            4, 6, 7,
            // 左
            1, 5, 6,
            1, 6, 2,
            // 右
            4, 0, 3,
            4, 3, 7,
            // 上
            4, 5, 1,
            4, 1, 0,
            // 下
            6, 7, 2,
            6, 2, 3,
    };

    GLuint VBO, VEO, VAO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VEO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5* sizeof(GLfloat), ((GLfloat *)0)+3);

//    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2* sizeof(GLfloat), 0);

    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindVertexArray(0);

    Shader shader("src/coordinate/vertex.glsl", "src/coordinate/fragment.glsl");

    GLint height, width;
    GLubyte * image1 = SOIL_load_image("texture/container.jpg",&width, &height, 0, SOIL_LOAD_RGB);


    GLuint textures[2];
    glGenTextures(2, textures);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLubyte * image2 = SOIL_load_image("texture/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
    glGenTextures(1, &textures[1]);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

//    printf("%d,%d\n", strlen(image1), strlen(image2));
    SOIL_free_image_data(image1);
    SOIL_free_image_data(image2);


    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.2, 0.3, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);


        shader.Use();
        glBindVertexArray(VAO);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glUniform1i(glGetUniformLocation(shader.Program, "ourTexture0"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glUniform1i(glGetUniformLocation(shader.Program, "ourTexture1"), 1);


        glm::mat4 model = glm::rotate(IdentityMat4, GLfloat(glfwGetTime()*glm::radians(55.0f)), glm::vec3(0.5, 1, 0));
        glm::mat4 view = glm::translate(IdentityMat4, glm::vec3(0, 0, -3.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(screenWidth)/float(screenHeight), 0.1f, 100.0f);
//        glm::mat4 trans = IdentityMat4;
//
//
//        trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0));
//        trans = glm::rotate(trans, (GLfloat)glfwGetTime()*3.0f, glm::vec3(0, 0, 1));
//
        GLuint loc = glGetUniformLocation(shader.Program, "model");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
        GLuint location = glGetUniformLocation(shader.Program, "view");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(view));

        location = glGetUniformLocation(shader.Program, "projection");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

//        trans = IdentityMat4;
//        GLfloat scale = sin(glfwGetTime())/2+0.5;
//        trans = glm::scale(trans, glm::vec3(scale, scale, 0));
//        trans = glm::translate(trans, glm::vec3(-0.5, -0.5, 0));
//
//        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(trans));
//
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}