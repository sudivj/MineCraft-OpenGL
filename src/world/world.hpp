#pragma once

#include "../../include/glad/glad.h"

#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../include/shader.h"
#include "../../include/camera.h"
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
    vector<chunk *> world_vector_map;
    // map<float, chunk *> sorted_world;

    Shader *shader;
    Camera *camera;

public:
    chunk *world_map[length][breadth];
    vector<vector<chunk *>> map;

    // void sort_world()
    // {
    //     for (unsigned int i = 0; i < world_vector_map.size(); i++)
    //     {
    //         sorted_world[(float)i] = world_vector_map[i];
    //     }
    // }

    void generate_world()
    {
        std::cout << "generating world..." << std::endl;
        for (int x = 0; x < length; x++)
        {
            vector<chunk *> temp_map;
            for (int y = 0; y < breadth; y++)
            {
                std::cout << "  > generating chunk: X: " << x << " - Y: " << y << " ..." << std::endl;
                temp_map.push_back(new chunk(x, y, shader, camera));
                this->world_map[x][y] = new chunk(x, y, shader, camera);
            }
            map.push_back(temp_map);
        }
    }

    int chunk_x_offset = 0;
    int chunk_y_offset = 0;
    void update_world(replace direction)
    {
        switch (direction)
        {
        case replace::CHUNK_FRONT:
            chunk_x_offset++;
            cout << "x " << chunk_x_offset << endl;
            for (int x = 0; x < length; x++)
            {
                for (int y = 0; y < breadth; y++)
                {
                    if (x < length - 1)
                    {
                        this->map[x][y] = map[x + 1][y];
                    }
                    else
                    {
                        this->map[x][y] = new chunk(x + chunk_x_offset, y + chunk_y_offset, shader, camera);
                    }
                }
            }
            break;
        
        case replace::CHUNK_RIGHT:
            chunk_y_offset++;
            cout << "y " << chunk_y_offset << endl;
            for (int x = 0; x < length; x++)
            {
                for (int y = 0; y < breadth; y++)
                {
                    if (y < breadth - 1)
                    {
                        this->map[x][y] = map[x][y + 1];
                    }
                    else
                    {
                        this->map[x][y] = new chunk(x, y + chunk_y_offset, shader, camera);
                    }
                }
            }
            break;
        case replace::CHUNK_BACK:
            chunk_x_offset--;
            cout << "x " << chunk_x_offset << endl;
            for (int x = 0; x < length; x++)
            {
                int r_x = length - (x + 1);
                for (int y = 0; y < breadth; y++)
                {
                    if (r_x > 0)
                    {
                        this->map[r_x][y] = map[r_x - 1][y];
                    }
                    else
                    {
                        this->map[0][y] = new chunk(0 - chunk_x_offset, y, shader, camera);
                    }
                }
            }
            break;
        case replace::CHUNK_LEFT:
            chunk_y_offset++;
            cout << "y " << chunk_y_offset << endl;
            for (int x = 0; x < length; x++)
            {
                for (int y = 0; y < breadth; y++)
                {
                    if (y < breadth - 1)
                    {
                        this->map[x][y] = map[x][y + 1];
                    }
                    else
                    {
                        this->map[x][y] = new chunk(x, y + chunk_y_offset, shader, camera);
                    }
                }
            }
            break;
        
        default:
            break;
        }
    }

    void draw_world()
    {
        shader->setInt("screen", 1);
        for (int x = 0; x < length; x++)
        {
            for (int y = 0; y < breadth; y++)
            {
                glEnable(GL_CULL_FACE);
                map[x][y]->draw_chunk();
            }
        }
        for (int x = 0; x < length; x++)
        {
            for (int y = 0; y < breadth; y++)
            {
                glDisable(GL_CULL_FACE);
                map[x][y]->draw_water();
            }
        }
    }

    world(Shader *shader_ID, Camera *cam_ID)
    {
        shader = shader_ID;
        camera = cam_ID;
        generate_world();
    }
};