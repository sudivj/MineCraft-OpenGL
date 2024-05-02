#pragma once

#include "../../include/glad/glad.h"
#include "../../include/FastNoiseLite.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include "block.hpp"
#include "../../include/shader.h"

using namespace std;

FastNoiseLite chunk_terrain;
FastNoiseLite chunk_terrainHeight;
class chunk
{
private:
    int c_x = 0;
    int c_y = 0;

    Shader *shader;
    Camera *camera;

    void generate_chunk()
    {
        for (int x = 0; x < i_l; x++)
        {
            for (int z = 0; z < i_b; z++)
            {
                float p_x = (float)(x + (this->c_x * i_l));
                float p_y = (float)(z + (this->c_y * i_l));
                float variation = chunk_terrainHeight.GetNoise((float)p_x, (float)p_y);
                int data = (int)floor((chunk_terrain.GetNoise((float)p_x, (float)p_y) + 1) * (i_h / 2)) + variation;
                data = data - 5;
                for (int y = 0; y < i_h; y++)
                {
                    if (y == 0)
                    {
                        this->chunk_map[x][z][y] = Block::SAND;
                        this->map.push_back({Block::SAND, glm::vec3(x, y, z)});
                    }
                    else if (y == data)
                    {
                        this->chunk_map[x][z][y] = Block::GRASS_BLOCK;
                        this->map.push_back({Block::GRASS_BLOCK, glm::vec3(x, y, z)});
                    }
                    else if (y >= data - 2 and y < data)
                    {
                        this->chunk_map[x][z][y] = Block::DIRT;
                        this->map.push_back({Block::DIRT, glm::vec3(x, y, z)});
                    }
                    else if (y < data)
                    {
                        this->chunk_map[x][z][y] = Block::STONE;
                        this->map.push_back({Block::STONE, glm::vec3(x, y, z)});
                    }
                    else if (y > data and y <= waterLevel and y != 0)
                    {
                        this->chunk_map[x][z][y] = Block::WATER;
                        this->water_blocks.push_back(glm::vec3(x + (c_x * i_l), y, z + (c_y * i_b)));
                    }
                    else
                    {
                        this->chunk_map[x][z][y] = Block::EMPTY;
                    }
                }
            }
        }
    }

public:
    Block chunk_map[i_l][i_b][i_h];
    vector<glm::vec3> water_blocks;
    vector<pair<Block, glm::vec3>> map;

    glm::vec3 world_position = glm::vec3(c_x * 16, 0.0, c_y * 16);
    glm::vec2 index_position = glm::vec2(c_x, c_y);

    chunk(int init_x, int init_y, Shader *shader_ID, Camera *camera_ID)
    {
        this->c_x = init_x;
        this->c_y = init_y;
        this->shader = shader_ID;
        this->camera = camera_ID;
        generate_chunk();
    }

    // Get block at (x, y, z) in this->chunk
    Block get_block(int x, int y, int z);
    // Draw this->chunk
    void draw_chunk();
    // Mesh block at (x, y, z) in this->chunk
    int draw_mesh(int x, int y, int z, Block name);
    // Draw this->water_blocks
    void draw_water();
};

int chunk::draw_mesh(int x, int y, int z, Block name)
{
    int block_face_count = 0;
    if (blocks[name]->type != blocks[get_block(x, y + 1, z)]->type)
    {
        draw_block(*shader, x, y, z, BlockFace::TOP, name, this->c_x, this->c_y);
        block_face_count++;
    }
    if (y == 0 or blocks[name]->type != blocks[get_block(x, y - 1, z)]->type)
    {
        draw_block(*shader, x, y, z, BlockFace::BOTTOM, name, this->c_x, this->c_y);
        block_face_count++;
    }
    if ((z == i_l - 1 or blocks[name]->type != blocks[get_block(x, y, z + 1)]->type) and name != Block::WATER)
    {
        draw_block(*shader, x, y, z, BlockFace::FRONT, name, this->c_x, this->c_y);
        block_face_count++;
    }
    if ((z == 0 or blocks[name]->type != blocks[get_block(x, y, z - 1)]->type) and name != Block::WATER)
    {
        draw_block(*shader, x, y, z, BlockFace::BACK, name, this->c_x, this->c_y);
        block_face_count++;
    }
    if ((x == i_b - 1 or blocks[name]->type != blocks[get_block(x + 1, y, z)]->type) and name != Block::WATER)
    {
        draw_block(*shader, x, y, z, BlockFace::SIDE_RIGHT, name, this->c_x, this->c_y);
        block_face_count++;
    }
    if ((x == 0 or blocks[name]->type != blocks[get_block(x - 1, y, z)]->type) and name != Block::WATER)
    {
        draw_block(*shader, x, y, z, BlockFace::SIDE_LEFT, name, this->c_x, this->c_y);
        block_face_count++;
    }

    return block_face_count;
}

Block chunk::get_block(int x, int y, int z)
{
    Block res = Block::EMPTY;
    res = this->chunk_map[x][z][y];
    return res;
}

void chunk::draw_chunk()
{
    for (int i = 0; i < map.size(); i++)
    {
        if (map[i].first == Block::GRASS)
        {
            draw_flora(*shader, map[i].second.x, map[i].second.y, map[i].second.z, map[i].first);
        }
        else
        {
            int res = draw_mesh(map[i].second.x, map[i].second.y, map[i].second.z, map[i].first);
            if (res == 0)
            {
                map.erase(map.begin() + i);
            }
        }
    }
    // draw_water();
    // cout << map.size() << endl;
}

void chunk::draw_water()
{
    glEnable(GL_BLEND);
    for (std::vector<glm::vec3>::iterator it = water_blocks.begin(); it != water_blocks.end(); ++it)
    {
        if (it->y = waterLevel)
        {
            draw_block(*shader, it->x, it->y, it->z, BlockFace::TOP, Block::WATER, 0, 0);
        }
    }
    glDisable(GL_BLEND);
}
