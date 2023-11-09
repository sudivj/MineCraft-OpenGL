// GL Libraries
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

// GL Math Libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Math Libraries
#include <vector>
#include <math.h>

#include <iostream>

// My Include Files
#include "../include/shader.h"
#include "../include/camera.h"
#include "../include/texture.hpp"
#include "../include/constants.hpp"
#include "world/world.hpp"
// #include "keyInput.hpp"
#include "cubeData.hpp"
#include "movement.hpp"


WorldGeneration world;

unsigned int VBO, VAO, EBO;

unsigned int texture1;
unsigned int texture2;

void drawObjects(Shader shaderClass, movement transformClass, Camera camera);

void render(GLFWwindow *window, Shader shaderClass, movement transformClass, Camera camera, float delta)
{        
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawObjects(shaderClass, transformClass, camera);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void initialization()
{
    glEnable(GL_DEPTH_TEST);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(testIndices), testIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    world.generateWorld();
}

void initTextures(Shader shaderClass)
{
    texture ourTexture;

    stbi_set_flip_vertically_on_load(true);

    texture1 = ourTexture.genTexture(1, "assets/container.jpg", glTYPE::GLRGB);
    texture2 = ourTexture.genTexture(1, "assets/awesomeface.png", glTYPE::GLRGBA);

    shaderClass.use();
    shaderClass.setInt("texture1", 0);
    shaderClass.setInt("texture2", 1);
}

void terminateProgram()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
}

void drawObjects(Shader shaderClass, movement transformClass, Camera camera)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO);

    shaderClass.use();
    shaderClass.setInt("texture1", 0);
    shaderClass.setInt("texture2", 1);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREENWIDTH / (float)SCREENHEIGHT, 0.1f, 100.0f);;
    shaderClass.setMat4("projection", projection);
    
    glm::mat4 view = camera.GetViewMatrix();
    shaderClass.setMat4("view", view);
    world.renderWorld(shaderClass);
}
