#pragma once

#include "../../include/glad/glad.h"

#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../include/shader.h"
#include "../../include/camera.h"
#include "../player/player_data.hpp"
#include "chunk.hpp"

const int length = 5;
const int breadth = 5;

enum replace
{
    CHUNK_FRONT,
    CHUNK_BACK,
    CHUNK_RIGHT,
    CHUNK_LEFT,
};

class world
{
private:

    int world_block_count = 0;

    Shader *shader;
    Camera *camera;
    player_data *player;

public:
    int x_off = (length - 1) / 2;
    int y_off = (breadth - 1) / 2;

    chunk *world_mid;

    chunk *world_map[length][breadth];

    void generate_world(void)
    {
        std::cout << "generating world..." << std::endl;
        for (int x = 0; x < length; x++)
        {
            for (int y = 0; y < breadth; y++)
            {
                std::cout << "  > generating chunk: X: " << x << " - Y: " << y << " ..." << std::endl;
                this->world_map[x][y] = new chunk(player->chunk_x + x - x_off, player->chunk_y + y - y_off, shader, camera);
            }
        }
        world_mid = world_map[x_off][y_off];
    }

    int chunk_x_offset = 0;
    int chunk_y_offset = 0;
    void update_world(replace direction)
    {
        //generate_world();
        switch (direction)
        {
        case replace::CHUNK_FRONT:
            chunk_x_offset++;
            for (int x = 0; x < length; x++)
            {
                for (int y = 0; y < breadth; y++)
                {
                    if (x < length - 1)
                    {
                        this->world_map[x][y] = world_map[x + 1][y];
                    }
                    else
                    {
                        this->world_map[x][y] = new chunk(player->chunk_x + x - x_off, player->chunk_y + y - y_off, shader, camera);
                    }
                }
            }
            break;
        
        case replace::CHUNK_RIGHT:
            chunk_y_offset++;
            for (int x = 0; x < length; x++)
            {
                for (int y = 0; y < breadth; y++)
                {
                    if (y < breadth - 1)
                    {
                        this->world_map[x][y] = world_map[x][y + 1];
                    }
                    else
                    {
                        this->world_map[x][y] = new chunk(player->chunk_x + x - x_off, player->chunk_y + y - y_off, shader, camera);
                    }
                }
            }
            break;
        case replace::CHUNK_BACK:
            chunk_x_offset--;
            for (int x = 0; x < length; x++)
            {
                int r_x = length - (x + 1);
                for (int y = 0; y < breadth; y++)
                {
                    if (r_x > 0)
                    {
                        this->world_map[r_x][y] = world_map[r_x - 1][y];
                    }
                    else
                    {
                        this->world_map[r_x][y] = new chunk(player->chunk_x + r_x - x_off, player->chunk_y + y - y_off, shader, camera);
                    }
                }
            }
            break;
        case replace::CHUNK_LEFT:
            chunk_y_offset++;
            for (int x = 0; x < length; x++)
            {
                for (int y = 0; y < breadth; y++)
                {
                    int r_y = breadth - (y + 1);
                    if (r_y > 0)
                    {
                        this->world_map[x][r_y] = world_map[x][r_y - 1];
                    }
                    else
                    {
                        this->world_map[x][r_y] = new chunk(player->chunk_x + x - x_off, player->chunk_y + r_y - y_off, shader, camera);
                    }
                }
            }
            break;
        
        default:
            break;
        }
        world_mid = world_map[x_off][y_off];
    }

    void draw_world()
    {
        world_block_count = 0;

        shader->setInt("screen", 1);
        for(int x = 0; x < length; x++) {
            for(int y = 0; y < length; y++) {
                glEnable(GL_CULL_FACE);
                world_map[x][y]->draw_chunk();
            }
        }
        for(int x = 0; x < length; x++) {
            for(int y = 0; y < length; y++) {
                glEnable(GL_CULL_FACE);
                world_map[x][y]->draw_water();
                world_block_count += world_map[x][y]->block_count;
            }
        }

        player->w_block_count = world_block_count;
    }

    world(Shader *shader_ID, Camera *cam_ID, player_data *player_ID)
    {
        shader = shader_ID;
        camera = cam_ID;
        player = player_ID;
        generate_world();
    }
};