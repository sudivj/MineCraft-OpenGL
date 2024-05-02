#pragma once

#include "../../include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include "../../include/shader.h"
#include "../../include/camera.h"
#include "../../include/constants.hpp"
#include "../player/player_data.hpp"

#include "text.hpp"

Shader *shader;
Camera *cam;

void set_shader_and_camera(Shader *ID, Camera *cam_ID) {
    shader = ID;
    cam = cam_ID;
}

void draw_gui(player_data &player) {
    RenderText(*shader,  "FPS: " + std::to_string(player.fps), 1790.0f, 1030.0f, 0.7f, glm::vec3(1.0, 1.0f, 1.0f));
    std::string cam_pos = std::to_string((int)player.x) + " " + std::to_string((int)player.y) + " " + std::to_string((int)player.z);
    std::string chunk_pos = std::to_string(player.chunk_x) + " " + std::to_string(player.chunk_y);
    std::string prev_chunk_pos = std::to_string(player.prev_chunk_x) + " " + std::to_string(player.prev_chunk_y);
    RenderText(*shader, cam_pos, 25.0f, 1030.0f, 0.7f, glm::vec3(1.0, 1.0f, 1.0f));
    RenderText(*shader, chunk_pos, 25.0f, 1000.0f, 0.7f, glm::vec3(1.0, 1.0f, 1.0f));
    RenderText(*shader, prev_chunk_pos, 25.0f, 970.0f, 0.7f, glm::vec3(1.0, 1.0f, 1.0f));
    RenderText(*shader, to_string(player.direction_x), 25.0f, 940.0f, 0.7f, glm::vec3(1.0, 1.0f, 1.0f));
}