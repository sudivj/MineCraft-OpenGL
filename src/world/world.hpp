#include "../../include/glad/glad.h"

#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "chunk.hpp"
#include "../../include/shader.h"

using namespace BLOCKS;

const int worldX = 1;
const int worldY = 1;

void renderBlockFace(Chunk chunk, int x, int y, int z)
{
    BLOCKS::BlockState block = chunk.returnBlockState(x, y, z + 1);
    if (block != BlockState::SOLID)
        glDrawArrays(GL_TRIANGLES, 0 * 6, 6);
    block = chunk.returnBlockState(x, y, z - 1);
    if (block != BlockState::SOLID)
        glDrawArrays(GL_TRIANGLES, 1 * 6, 6);
    block = chunk.returnBlockState(x + 1, y, z);
    if (block != BlockState::SOLID)
        glDrawArrays(GL_TRIANGLES, 2 * 6, 6);
    block = chunk.returnBlockState(x - 1, y, z);
    if (block != BlockState::SOLID)
        glDrawArrays(GL_TRIANGLES, 3 * 6, 6);
    block = chunk.returnBlockState(x, y + 1, z);
    if (block != BlockState::SOLID)
        glDrawArrays(GL_TRIANGLES, 4 * 6, 6);
    block = chunk.returnBlockState(x, y - 1, z);
    if (block != BlockState::SOLID)
        glDrawArrays(GL_TRIANGLES, 5 * 6, 6);

    // glCullFace(GL_BACK);
}

bool blockShade(Chunk chunk, int x, int y, int z)
{
    int shadows = 0;
    shadows += (bool)(chunk.returnBlockState(x, y + 1, z - 1) == BlockState::SOLID);
    shadows += (bool)(chunk.returnBlockState(x, y + 1, z + 1) == BlockState::SOLID);
    shadows += (bool)(chunk.returnBlockState(x - 1, y + 1, z) == BlockState::SOLID);
    shadows += (bool)(chunk.returnBlockState(x + 1, y + 1, z) == BlockState::SOLID);
    shadows += (bool)(chunk.returnBlockState(x - 1, y + 1, z + 1) == BlockState::SOLID);
    shadows += (bool)(chunk.returnBlockState(x + 1, y + 1, z - 1) == BlockState::SOLID);
    shadows += (bool)(chunk.returnBlockState(x + 1, y + 1, z + 1) == BlockState::SOLID);
    shadows = shadows * (bool)(chunk.returnBlockState(x, y + 1, z) == BlockState::AIR);

    return (shadows > 0);
}

class WorldGeneration
{
public:
    void generateWorld()
    {
        for (int i = 0; i < worldX; i++)
        {
            for (int j = 0; j < worldY; j++)
            {
                world[i][j] = new Chunk(i, j);
                world[i][j]->generateChunk();
            }
        }
    }
    void renderChunk(Chunk current_chunk, unsigned int shaderID, int wLen, int wWid)
    {
        for (int x = 0; x < l; x++)
        {
            for (int z = 0; z < b; z++)
            {
                for (int y = 0; y < h; y++)
                {
                    // int currentBlockstate = current_chunk.returnBlockState(x, y, z);
                    if (current_chunk.returnBlockState(x, y, z) == BlockState::SOLID)
                    {
                        float alphaMap[] = {0.15, 0.20, 0.25, 0.30, 0.1 + ((float)0.5 * (float)blockShade(current_chunk, x, y, z)), 0.4};
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, glm::vec3((float)(x + 1) + (l * wLen), (float)y, (float)(z + 1) + (b * wWid)));
                        // shader.setMat4("model", model);
                        glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                        // renderBlockFace(current_chunk, x, y, z);
                        for (int i = 0; i < 6; i++)
                        {
                            float range = -0.15;
                            glUniform1f(glGetUniformLocation(shaderID, "alpha"), alphaMap[i] + range);
                            glDrawArrays(GL_TRIANGLES, i * 6, 6);
                        }
                    }
                }
            }
        }
    }
    void renderWorld(Shader shader)
    {
        unsigned int shaderID = shader.ID;
        for (int i = 0; i < worldX; i++)
        {
            for (int j = 0; j < worldY; j++)
            {
                renderChunk(*world[i][j], shaderID, i, j);
            }
        }
    }

private:
    Chunk *world[worldX][worldY];
};