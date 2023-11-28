#include "../../include/glad/glad.h"

#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "chunk.hpp"
#include "../../include/shader.h"

using namespace BLOCKS;
using namespace std;

const int worldX = 3;
const int worldY = 3;

int playerChunk[] = {0, 0};
long int world[worldX][worldY];

float wOff = 0;

int x, y, z;

unsigned int shaderID;

void updatePlayerChunk(int camx, int camz)
{
    // if ((camx > playerChunk[0] * 16) || (camx < playerChunk[0] * 16))
    // {
    //     cout << playerChunk[0] << " " << playerChunk[1] << endl;
    // }
    // if ((camz > playerChunk[1] * 16) || (camz < playerChunk[0] * 16))
    // {
    //     cout << playerChunk[0] << " " << playerChunk[1] << endl;
    // }
    playerChunk[0] += (camx > (playerChunk[0] * 16) + 16);
    playerChunk[0] -= (camx < (playerChunk[0] * 16));
    playerChunk[1] += (camz > (playerChunk[1] * 16) + 16);
    playerChunk[1] -= (camz < (playerChunk[1] * 16));
}

// Send Matrix To Vertex Shader
void setModelMetrix(int indexX, int indexY)
{
    glm::mat4 model = glm::mat4(1.0f);
    float sendX = (float)(x + 1) + (l * indexX);
    float sendY = (float)y;
    float sendZ = (float)(z + 1) + (b * indexY);
    model = glm::translate(model, glm::vec3(sendX, sendY, sendZ));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

// Check Block Shadow
bool blockShade(int i, int j)
{
    int shadows = 0;
    shadows += (int)(returnBlockState(x, y + 1, z - 1, i, j) == BlockState::SOLID);
    shadows += (int)(returnBlockState(x, y + 1, z + 1, i, j) == BlockState::SOLID);
    shadows += (int)(returnBlockState(x - 1, y + 1, z, i, j) == BlockState::SOLID);
    shadows += (int)(returnBlockState(x + 1, y + 1, z, i, j) == BlockState::SOLID);
    shadows += (int)(returnBlockState(x - 1, y + 1, z - 1, i, j) == BlockState::SOLID);
    shadows += (int)(returnBlockState(x - 1, y + 1, z + 1, i, j) == BlockState::SOLID);
    shadows += (int)(returnBlockState(x + 1, y + 1, z - 1, i, j) == BlockState::SOLID);
    shadows += (int)(returnBlockState(x + 1, y + 1, z + 1, i, j) == BlockState::SOLID);    
    shadows = shadows * (bool)(returnBlockState(x, y + 1, z, i, j) == BlockState::AIR);

    return (shadows > 0);
}

// Face Culling
int getBlockNeighbours(int i, int j, int side)
{
    bool block[6] = {0, 0, 0, 0, 0, 0};
    block[0] = (bool)((returnBlockState(x, y, z + 1, i, j) == BlockState::SOLID) && z != (j * b) + (b + 1));
    block[1] = (bool)((returnBlockState(x, y, z - 1, i, j) == BlockState::SOLID) && z != 0);
    block[2] = (bool)((returnBlockState(x + 1, y, z, i, j) == BlockState::SOLID) && x != (i * l) + (l - 1));
    block[3] = (bool)((returnBlockState(x - 1, y, z, i, j) == BlockState::SOLID) && x != 0);
    block[4] = (bool)((returnBlockState(x, y + 1, z, i, j) == BlockState::SOLID));
    block[5] = (bool)((returnBlockState(x, y - 1, z, i, j) == BlockState::SOLID) && y != 0);
    return (block[side] != true);
}

//Get number of neighbouring blocks
int getBlockNeighbours(int i, int j)
{
    bool block[6] = {0, 0, 0, 0, 0, 0};
    block[0] = (bool)((returnBlockState(x, y, z + 1, i, j) == BlockState::SOLID) && z != (j * b) + (b + 1));
    block[1] = (bool)((returnBlockState(x, y, z - 1, i, j) == BlockState::SOLID) && z != 0);
    block[2] = (bool)((returnBlockState(x + 1, y, z, i, j) == BlockState::SOLID) && x != (i * l) + (l - 1));
    block[3] = (bool)((returnBlockState(x - 1, y, z, i, j) == BlockState::SOLID) && x != 0);
    block[4] = (bool)((returnBlockState(x, y + 1, z, i, j) == BlockState::SOLID));
    block[5] = (bool)((returnBlockState(x, y - 1, z, i, j) == BlockState::SOLID) && y != 0);
    return block[0] + block[1] + block[2] + block[3] + block[4] + block[5];
}

void drawBlock(int i, int j)
{
    float alphaMap[] = {0.15, 0.20, 0.25, 0.30, 0.1 + (float)(0.1 * blockShade(i, j)), 0.4};
    float range = -0.15;
    if(getBlockNeighbours(i, j) != 6)
    {
        for (int f = 1; f < 6 + 1; f++)
        {
            glUniform1f(glGetUniformLocation(shaderID, "alpha"), alphaMap[f - 1] + range);
            setModelMetrix(i, j);
            glDrawArrays(GL_TRIANGLES, (f * getBlockNeighbours(i, j, f - 1)) * 6, 6);
        }
    }
}

// Render Current Chunk
void renderChunk(int indexX, int indexY)
{
    for (x = 0; x < l; x++)
    {
        for (z = 0; z < b; z++)
        {
            for (y = 0; y < h; y++)
            {
                if (returnBlockState(x + wOff, y, z + wOff, indexX, indexY) == BlockState::SOLID)
                {
                    drawBlock(indexX, indexY);
                }
            }
        }
    }
}

void renderWorld(Shader shader)
{
    shaderID = shader.ID;
    int startI = 0;
    int startJ = 0;
    if (worldX != 1)
    {
        startI = (worldX - 1) / 2;
    }
    if (worldX != 1)
    {
        startJ = (worldY - 1) / 2;
    }
    for (int i = -startI; i < startI + 1; i++)
    {
        for (int j = -startJ; j < startJ + 1; j++)
        {
            renderChunk(playerChunk[0] + i, playerChunk[1] + j);
        }
        
    }
}

// Generate Map
void generateWorld()
{
    
}