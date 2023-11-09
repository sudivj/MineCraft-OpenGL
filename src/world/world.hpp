#include "../../include/glad/glad.h"

#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "chunk.hpp"
#include "../../include/shader.h"

using namespace BLOCKS;

const int worldX = 6;
const int worldY = 6;

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
                            glm::mat4 model = glm::mat4(1.0f);
                            model = glm::translate(model, glm::vec3((float)(x + 1) + (l * wLen), (float)y, (float)(z + 1) + (b * wWid)));
                            // shader.setMat4("model", model);
                            glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                            glDrawArrays(GL_TRIANGLES, 0, 36);
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