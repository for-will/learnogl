//
// Created by puffer on 2018/11/16.
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
#include <vector>
#include <zconf.h>

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_movement();

const GLuint  WIDTH = 800, HEIGHT = 600;

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


int main(void){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "test camera", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    glewExperimental = GL_TRUE;
    glewInit();

    GLint screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    GLfloat vertexes[] = {
            -0.5,   0.5,   0.5,     1.0,   1.0,
            -0.5,  -0.5,   0.5,     0.0,   1.0,
            -0.5,  -0.5,  -0.5,     0.0,   0.0,
            -0.5,   0.5,  -0.5,     1.0,   0.0,

            0.5,   0.5,   0.5,     1.0,   1.0,
            0.5,  -0.5,   0.5,     0.0,   1.0,
            0.5,  -0.5,  -0.5,     0.0,   0.0,
            0.5,   0.5,  -0.5,     1.0,   0.0,


            0.5,  -0.5,   0.5,     1.0,   1.0,
            -0.5,  -0.5,   0.5,     0.0,   1.0,
            -0.5,  -0.5,  -0.5,     0.0,   0.0,
            0.5,  -0.5,  -0.5,     1.0,   0.0,

            0.5,   0.5,   0.5,     1.0,   1.0,
            -0.5,   0.5,   0.5,     0.0,   1.0,
            -0.5,   0.5,  -0.5,     0.0,   0.0,
            0.5,   0.5,  -0.5,     1.0,   0.0,


            0.5,   0.5,  -0.5,     1.0,   1.0,
            -0.5,   0.5,  -0.5,     0.0,   1.0,
            -0.5,  -0.5,  -0.5,     0.0,   0.0,
            0.5,  -0.5,  -0.5,     1.0,   0.0,

            0.5,   0.5,   0.5,     1.0,   1.0,
            -0.5,   0.5,   0.5,     0.0,   1.0,
            -0.5,  -0.5,   0.5,     0.0,   0.0,
            0.5,  -0.5,   0.5,     1.0,   0.0,

    };

    float vals[] = {
            1, 1,
            0, 1,
            0, 0,
            1, 0
    };
    for(int i =0; i < 3; i++){
        for(int k=0; k < 2; k++) {
            for(int j = 0; j < 4; j++){
                std::vector<float> l = {vals[j * 2], vals[j * 2 + 1]};
                l.insert(l.begin() + i, k);
                printf("%5.1f, %5.1f, %5.1f,   %5.1f, %5.1f,\n", l[0]-0.5, l[1]-0.5, l[2]-0.5, vals[j * 2], vals[j * 2 + 1]);
            }
            printf("\n");
        }
        printf("\n");
    }

    GLuint indices[36];

    for(int i = 0; i < 6; i++){
        indices[i*6+0] = i*4+0;
        indices[i*6+1] = i*4+1;
        indices[i*6+2] = i*4+2;
        indices[i*6+3] = i*4+0;
        indices[i*6+4] = i*4+2;
        indices[i*6+5] = i*4+3;
    }

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


    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

//    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)){

        // Calculate deltatime of current frame
        GLfloat  currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        do_movement();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2, 0.3, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


        // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glUniform1i(glGetUniformLocation(shader.Program, "ourTexture0"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glUniform1i(glGetUniformLocation(shader.Program, "ourTexture1"), 1);

        // Activate shader
        shader.Use();


        glm::mat4 model = glm::rotate(IdentityMat4, GLfloat(glfwGetTime()*glm::radians(55.0f)), glm::vec3(0.5, 1, 0));

        // Camera/View transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // Projection
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), float(screenWidth)/float(screenHeight), 0.1f, 100.0f);

//        GLuint location;

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(shader.Program, "model");
        GLint viewLoc = glGetUniformLocation(shader.Program, "view");
        GLint projLoc= glGetUniformLocation(shader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        for (int i =0; i < 10; i++) {
            GLfloat angle = glm::radians(20.0*i);
            if (i%3==0){
                angle = glm::radians(glfwGetTime()*25);
            }
            model = glm::translate(IdentityMat4, cubePositions[i]);
            model = glm::rotate(model, angle, glm::vec3(1, 0.5, 0.3));

//            GLuint loc = glGetUniformLocation(shader.Program, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        usleep(1000000/60.0f);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action==GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key >= 0 && key < 1024){
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
void do_movement(){

    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
