#pragma once

#include "../../include/glad/glad.h"

#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../include/shader.h"
#include "../../include/camera.h"
#include "block_data.hpp"

const int i_l = 16;
const int i_b = 16;
const int i_h = 32;

const int waterLevel = 10;

void draw_block(Shader &shader, float x, float y, float z, BlockFace face, Block name, int c_x, int c_y)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3((float)((c_x * i_b) + x), (float)y, (float)((c_y * i_l) + z)));
    shader.setMat4("model", model);
    switch (face)
    {
    case BlockFace::BACK:
        shader.setFloat("brightness", 0.05);
        shader.setInt("textureIndex", blocks[name]->blockTextureMap[face]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        break;
    case BlockFace::FRONT:
        shader.setFloat("brightness", 0.02);
        shader.setInt("textureIndex", blocks[name]->blockTextureMap[face]);
        glDrawArrays(GL_TRIANGLES, 6, 6);
        break;
    case BlockFace::SIDE_LEFT:
        shader.setFloat("brightness", 0.04);
        shader.setInt("textureIndex", blocks[name]->blockTextureMap[face]);
        glDrawArrays(GL_TRIANGLES, 12, 6);
        break;
    case BlockFace::SIDE_RIGHT:
        shader.setFloat("brightness", 0.03);
        shader.setInt("textureIndex", blocks[name]->blockTextureMap[face]);
        glDrawArrays(GL_TRIANGLES, 18, 6);
        break;
    case BlockFace::BOTTOM:
        shader.setFloat("brightness", 0.1);
        shader.setInt("textureIndex", blocks[name]->blockTextureMap[face]);
        glDrawArrays(GL_TRIANGLES, 24, 6);
        break;
    case BlockFace::TOP:
        shader.setFloat("brightness", 0.01);
        shader.setInt("textureIndex", blocks[name]->blockTextureMap[face]);
        glDrawArrays(GL_TRIANGLES, 30, 6);
        break;
    }
}

void draw_flora(Shader& shader, int x, int y, int z, Block name) {
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3((float)x, (float)y, (float)z));
    model = glm::rotate(model, glm::radians((float)45), glm::vec3(0.0, 1.0, 0.0));
    shader.setMat4("model", model);
    shader.setFloat("brightness", 0.0);
    shader.setInt("textureIndex", blocks[name]->blockTextureMap[BlockFace::PLAIN]);
    glDrawArrays(GL_TRIANGLES, 36, 42);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3((float)x, (float)y, (float)z));
    model = glm::rotate(model, glm::radians((float)135), glm::vec3(0.0, 1.0, 0.0));
    shader.setMat4("model", model);
    shader.setFloat("brightness", 0.0);
    shader.setInt("textureIndex", blocks[name]->blockTextureMap[BlockFace::PLAIN]);
    glDrawArrays(GL_TRIANGLES, 36, 42);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
}