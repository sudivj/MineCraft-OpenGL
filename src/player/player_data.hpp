#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include "../../include/camera.h"

struct player_data {

    glm::vec3 *position_ptr;

    int w_block_count = 0;

    bool changed_position;

    int pre_x = 0;
    int pre_y = 0;
    int pre_z = 0;

    int x = 0;
    int y = 0;
    int z = 0;
    glm::vec3 position;

    int x_in_chunk = 0;
    int z_in_chunk = 0;

    int chunk_x = 0;
    int chunk_y = 0;
    glm::vec2 chunk_position;

    int prev_chunk_x = 0;
    int prev_chunk_y = 0;
    glm::vec2 prev_chunk_position;

    int direction_x = 0;
    int direction_y = 0;

    int fps;

    player_data(glm::vec3 *pos_ptr) {
        position_ptr = pos_ptr;
    }

    void update_position_in_chunk() {
        this->x_in_chunk = (int)this->position.x % 16;
        this->z_in_chunk = (int)this->position.z % 16;
    }

    void update_position() {
        this->pre_x = this->x;
        this->pre_y = this->y;
        this->pre_z = this->z;
    }

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

    void update_chunk_position() {
        if (this->x < 0) {
            this->chunk_x = ceil(this->x / 16) - 1;
        } else {
            this->chunk_x = floor(this->x / 16);
        }
        if (this->z < 0) {
            this->chunk_y = ceil(this->z / 16) - 1;
        } else {
            this->chunk_y = floor(this->z / 16);
        }
    }
};