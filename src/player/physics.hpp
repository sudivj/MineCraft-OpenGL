#pragma once

#include "../../include/camera.h"
#include "../world/world.hpp"
#include "player_data.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

const float gravitational_acceleration = 0.08f;

class player_physics
{
private:
    player_data *player_data_ptr;
    world *world_ptr;

    bool isFalling = true;
    bool isBelowEmpty = false;
    bool isPositionEmpty = false;
    bool isVerticallyColliding = false;
    float gravitational_speed = 0.0f;

public:
    player_physics(player_data *player_ptr, world *w_ptr)
    {
        player_data_ptr = player_ptr;
        world_ptr = w_ptr;
    }
    void gravity();
    void check_vertical_collision();
    //void correct_postion_y();
    bool is_Below_Empty(glm::vec3 pos);
    bool is_Position_Empty(glm::vec3 pos);
};

void player_physics::gravity()
{
    if(player_data_ptr->y < total_h)
    {
        isBelowEmpty = is_Below_Empty(glm::vec3(player_data_ptr->x_in_chunk, player_data_ptr->y, player_data_ptr->z_in_chunk));
        isPositionEmpty = is_Position_Empty(glm::vec3(player_data_ptr->x_in_chunk, player_data_ptr->y, player_data_ptr->z_in_chunk));
        std::cout << player_data_ptr->x_in_chunk << " " << player_data_ptr->y << " " << player_data_ptr->z_in_chunk << std::endl;
        std::cout << isBelowEmpty << std::endl;
        std::cout << isPositionEmpty << std::endl;
    }
    if (isFalling)
    {
        gravitational_speed -= gravitational_acceleration;
        player_data_ptr->position_ptr->y += gravitational_speed;
    }
    else
    {
        gravitational_speed = 0;
    }
    check_vertical_collision();
}

void player_physics::check_vertical_collision()
{
    if (player_data_ptr->position.y < total_h)
    {
        if (isBelowEmpty)
        {
            isFalling = false;
        }
    } else {
        isBelowEmpty = false;
        isFalling = true;
    }
}

bool player_physics::is_Below_Empty(glm::vec3 pos)
{
    Block block = world_ptr->world_mid->get_block((int)pos.x, (int)pos.y - 2, (int)pos.z);
    //std::cout << block << std::endl;
    if (block == Block::EMPTY)
    {
        return true;
    } else {
        return false;
    }
}

bool player_physics::is_Position_Empty(glm::vec3 pos)
{
    Block block = world_ptr->world_mid->get_block((int)pos.x, (int)pos.y - 1, (int)pos.z);
    //std::cout << block << std::endl;
    if (block == Block::EMPTY)
    {
        return true;
    } else {
        return false;
    }
}