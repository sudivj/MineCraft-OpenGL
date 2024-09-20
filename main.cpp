// #include <bits/stdc++.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>

// #include "src/keyInput.hpp"
#include "src/world/world.hpp"
#include "src/cubeData.hpp"
#include "include/texture.hpp"
#include "include/constants.hpp"
#include "src/GUI/gui.hpp"
#include "src/player/player_data.hpp"
#include "src/player/physics.hpp"
#include "console.hpp"
#include <unistd.h>

using namespace std;

void call_update_world(bool chunk, world &w);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, Shader &shader, world &w);
int get_FPS();

// Camera camera(glm::vec3((float)(16 * 2 + 16 / 2), 15.0f, (float)(16 * 2 + 16 / 2)));
Camera camera(glm::vec3(0.0, 0.0, 0.0));
float lastX = SCREENWIDTH / 2.0f;
float lastY = SCREENHEIGHT / 2.0f;
bool firstMouse = true;

bool space_pressed = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastTime = 0.0f;

player_data player(&camera.Position);

glm::ivec2 lastChunk;
glm::ivec2 currentChunk;

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "MineCraft - OpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader("src/Shaders/shaderVert.glsl", "src/Shaders/shaderFrag.glsl");

    unsigned int VBO, VAO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    init_freetype();
    init_text_vertex();
    set_shader_and_camera(&ourShader, &camera);

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    unsigned int atlas;

    texture ourTex;

    atlas = ourTex.genTexture(1, "assets/block_atlas.png", glTYPE::GLRGBA);
    ourShader.setInt("atlas", 0);

    init_blocks();

    camera.Position.y = 50.0f;

    world w(&ourShader, &camera, &player);
    player_physics physics(&player, &w);

    clear();

    while (!glfwWindowShouldClose(window))
    {
        //console(player);

        cout << player.w_block_count << endl;

        player.x = (int)camera.Position.x;
        player.y = (int)camera.Position.y;
        player.z = (int)camera.Position.z;
        player.update_position_in_chunk();
        player.update_chunk_position();
        player.position = camera.Position;
        call_update_world(player.update_chunk(), w);

        //physics.gravity();

        glClearColor(0.212f, 0.78f, 0.949f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlas);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        player.fps = get_FPS();

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, ourShader, w);

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREENWIDTH / (float)SCREENHEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        w.draw_world();
        ourShader.setInt("screen", 2);

        projection = glm::ortho(0.0f, static_cast<float>(SCREENWIDTH), 0.0f, static_cast<float>(SCREENHEIGHT));
        ourShader.setMat4("projection", projection);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        draw_gui(player);

        glBindVertexArray(VAO);
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow *window, Shader &shader, world &w)
{
    float velocity = camera.MovementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Position += glm::vec3(camera.Front.x, sin(glm::radians(0.0f)), camera.Front.z) * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Position -= glm::vec3(camera.Front.x, 0.0f, camera.Front.z) * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

int get_FPS()
{
    float currentTime = static_cast<float>(glfwGetTime());
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    // std::cout << "FPS: " << (int)(1 / deltaTime) << "/" << deltaTime << std::endl;
    return (int)(1 / deltaTime);
}

void call_update_world(bool chunk, world &w)
{
    if (chunk)
    {
        if (player.direction_x > 0) w.update_world(replace::CHUNK_FRONT);
        if (player.direction_x < 0) w.update_world(replace::CHUNK_BACK);
        if (player.direction_y > 0) w.update_world(replace::CHUNK_RIGHT);
        if (player.direction_y < 0) w.update_world(replace::CHUNK_LEFT);
    }
}