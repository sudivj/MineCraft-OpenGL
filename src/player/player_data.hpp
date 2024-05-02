#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

struct player_data {
    int x = 0;
    int y = 0;
    int z = 0;
    glm::vec3 position;

    int chunk_x = 0;
    int chunk_y = 0;
    glm::vec2 chunk_position;

    int prev_chunk_x = 0;
    int prev_chunk_y = 0;
    glm::vec2 prev_chunk_position;

    int direction_x = 0;
    int direction_y = 0;

    int fps;

    bool update_chunk() {
        update_vectors();
        if ((chunk_position.x != prev_chunk_position.x) or (chunk_position.y != prev_chunk_position.y)) {
            direction_x = chunk_x - prev_chunk_x;
            direction_y = chunk_y - prev_chunk_y;
            prev_chunk_x = chunk_x;
            prev_chunk_y = chunk_y;
            update_vectors();
            return true;
        }
        return false;
    }

    void update_vectors() {
        this->chunk_position = glm::vec2(chunk_x, chunk_y);
        this->prev_chunk_position = glm::vec2(prev_chunk_x, prev_chunk_y);
    }

    void update_direction() {
        direction_x = chunk_x - prev_chunk_x;
        direction_y = chunk_y - prev_chunk_y;
    }
};