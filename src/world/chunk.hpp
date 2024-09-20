#pragma once

#include "../../include/glad/glad.h"
#include "../../include/FastNoiseLite.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <map>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include "block.hpp"
#include "../../include/shader.h"

using namespace std;

FastNoiseLite chunk_terrain;
FastNoiseLite chunk_terrainHeight;
FastNoiseLite ocean_terrain;
class chunk
{
private:
    int c_x = 0;
    int c_y = 0;

    Shader *shader;
    Camera *camera;

    bool check_around(int x, int y, int z, Block check_for)
    {
        if (this->get_block(x + 1, y, z) == check_for)
        {
            return true;
        }
        else if (this->get_block(x - 1, y, z) == check_for)
        {
            return true;
        }
        else if (this->get_block(x, y + 1, z) == check_for)
        {
            return true;
        }
        else if (this->get_block(x, y - 1, z) == check_for)
        {
            return true;
        }
        else if (this->get_block(x, y, z + 1) == check_for)
        {
            return true;
        }
        else if (this->get_block(x, y, z - 1) == check_for)
        {
            return true;
        }

        return false;
    }

    // Terrain Generation

    void generate_terrain()
    {
        for (int x = 0; x < i_l; x++)
        {
            for (int z = 0; z < i_b; z++)
            {
                float p_x = (float)(x + (this->c_x * i_l));
                float p_y = (float)(z + (this->c_y * i_l));
                float variation = chunk_terrainHeight.GetNoise((float)p_x, (float)p_y);
                int data = (int)floor((chunk_terrain.GetNoise((float)p_x, (float)p_y) + 1) * (i_h / 2)) * variation;
                data = base_h + data;
                for (int y = 0; y < total_h; y++)
                {
                    if (y <= data)
                    {
                        this->chunk_map[x][z][y] = Block::STONE;
                    }
                    else if (y > data and y <= waterLevel)
                    {
                        this->chunk_map[x][z][y] = Block::WATER;
                    }
                    else
                    {
                        this->chunk_map[x][z][y] = Block::EMPTY;
                    }
                }
            }
        }
    }

    // Biome Generation

    void generate_biomes()
    {
        int mesh_index = 0;

        for (int x = 0; x < i_l; x++)
        {
            for (int z = 0; z < i_b; z++)
            {
                for (int y = 0; y < total_h; y++)
                {
                    if (this->get_block(x, y, z) != Block::EMPTY)
                    {
                        if (this->chunk_map[x][z][y + 1] == Block::EMPTY and this->get_block(x, y, z) != Block::WATER)
                        {
                            this->chunk_map[x][z][y] = Block::GRASS_BLOCK;
                        }
                        else if (this->check_around(x, y, z, Block::WATER) and this->chunk_map[x][z][y] != Block::WATER)
                        {
                            float data = ocean_terrain.GetNoise((float)((x) + (c_x * i_l)), (float)((z) + (c_y * i_l)));
                            if (data < 0.1)
                            {
                                this->chunk_map[x][z][y] = Block::DIRT;
                            }
                            else
                            {
                                this->chunk_map[x][z][y] = Block::SAND;
                            }
                        }
                        if (this->get_block(x, y, z) != Block::WATER)
                        {
                            this->map.push_back({this->get_block(x, y, z), glm::vec3(x, y, z)});
                            this->chunk_mesh.push_back(new block_mesh(this->get_block(x, y, z) , glm::vec3(x, y, z)));
                            if (create_mesh(chunk_mesh[mesh_index]))
                            {
                                chunk_mesh.erase(chunk_mesh.begin() + mesh_index);
                            }
                            else
                            {
                                mesh_index++;
                            }
                        }
                        else
                        {
                            if(y == waterLevel)
                                this->water_blocks.push_back(glm::vec3(x + (c_x * i_l), y, z + (c_y * i_b)));
                        }
                    }
                }
            }
        }
    }

    // Chunk Generation

    void generate_chunk()
    {
        generate_terrain();
        generate_biomes();
    }

public:
    Block chunk_map[i_l][i_b][total_h];
    vector<glm::vec3> water_blocks;
    vector<pair<Block, glm::vec3>> map;
    vector<block_mesh *> chunk_mesh;
    int block_count = 0;

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
    // Create mesh
    bool create_mesh(block_mesh* b_mesh);
    // Mesh block at (x, y, z) in this->chunk
    void draw_mesh(block_mesh* mesh);
    // Draw this->water_blocks
    void draw_water();
};

bool chunk::create_mesh(block_mesh* b_mesh)
{
    int x = b_mesh->pos.x;
    int y = b_mesh->pos.y;
    int z = b_mesh->pos.z;
    Block name = b_mesh->name;
    int block_face_count = 0;
    if (blocks[name]->type != blocks[get_block(x, y + 1, z)]->type)
    {
        //draw_block(*shader, x, y, z, BlockFace::TOP, name, this->c_x, this->c_y);
        b_mesh->faces.push_back(BlockFace::TOP);
        block_face_count++;
    }
    if (y == 0 or blocks[name]->type != blocks[get_block(x, y - 1, z)]->type)
    {
        //draw_block(*shader, x, y, z, BlockFace::BOTTOM, name, this->c_x, this->c_y);
        b_mesh->faces.push_back(BlockFace::BOTTOM);
        block_face_count++;
    }
    if ((z == i_l - 1 or blocks[name]->type != blocks[get_block(x, y, z + 1)]->type) and name != Block::WATER)
    {
        //draw_block(*shader, x, y, z, BlockFace::FRONT, name, this->c_x, this->c_y);
        b_mesh->faces.push_back(BlockFace::FRONT);
        block_face_count++;
    }
    if ((z == 0 or blocks[name]->type != blocks[get_block(x, y, z - 1)]->type) and name != Block::WATER)
    {
        //draw_block(*shader, x, y, z, BlockFace::BACK, name, this->c_x, this->c_y);
        b_mesh->faces.push_back(BlockFace::BACK);
        block_face_count++;
    }
    if ((x == i_b - 1 or blocks[name]->type != blocks[get_block(x + 1, y, z)]->type) and name != Block::WATER)
    {
        //draw_block(*shader, x, y, z, BlockFace::SIDE_RIGHT, name, this->c_x, this->c_y);
        b_mesh->faces.push_back(BlockFace::SIDE_RIGHT);
        block_face_count++;
    }
    if ((x == 0 or blocks[name]->type != blocks[get_block(x - 1, y, z)]->type) and name != Block::WATER)
    {
        //draw_block(*shader, x, y, z, BlockFace::SIDE_LEFT, name, this->c_x, this->c_y);
        b_mesh->faces.push_back(BlockFace::SIDE_LEFT);
        block_face_count++;
    }
    if(block_face_count == 0)
    {
        return true;
    }

    return false;
}

Block chunk::get_block(int x, int y, int z)
{
    Block res = Block::EMPTY;
    res = this->chunk_map[x][z][y];
    return res;
}

void chunk::draw_chunk()
{
    block_count = 0;
    for (int i = 0; i < chunk_mesh.size(); i++)
    {
        glm::vec3 b_pos;
        b_pos.x = chunk_mesh[i]->pos.x + (c_x * i_b);
        b_pos.y = chunk_mesh[i]->pos.y;
        b_pos.z = chunk_mesh[i]->pos.z + (c_y * i_l);
        if (glm::dot(b_pos, camera->Front) + glm::length(camera->Position) >= 0)
        {
            draw_mesh(chunk_mesh[i]);
            block_count++;
        }
    }
    // block_count = chunk_mesh.size();
}

void chunk::draw_water()
{
    glEnable(GL_BLEND);
    for (std::vector<glm::vec3>::iterator it = water_blocks.begin(); it != water_blocks.end(); ++it)
    {
        if (it->y = waterLevel)
        {
            if (glm::dot(*it, camera->Front) + glm::length(camera->Position) >= 0)
            {
                draw_block(*shader, it->x, it->y, it->z, BlockFace::TOP, Block::WATER, 0, 0);
                block_count++;
            }
        }
    }
    //block_count += water_blocks.size();
}

void chunk::draw_mesh(block_mesh* mesh)
{
    for (int f = 0; f < mesh->faces.size(); f++)
    {
        draw_block(*shader, mesh->pos.x, mesh->pos.y, mesh->pos.z, mesh->faces[f], mesh->name, this->c_x, this->c_y);
    }
}
