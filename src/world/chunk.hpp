#include "../../include/FastNoiseLite.h"
#include <math.h>
#include <iostream>
#include "block.hpp"

FastNoiseLite terrain;
FastNoiseLite terrainHeight;

int curChunk = 1;

const int l = 16;
const int b = 16;
const int h = 20;

namespace BLOCKS{
    enum BlockState {
        NULLBLOCK,
        AIR,
        SOLID,
        WATER
    };
}

void initWorld() {
    terrain.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    terrain.SetFractalType(FastNoiseLite::FractalType_FBm);
    terrainHeight.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    terrainHeight.SetFractalType(FastNoiseLite::FractalType_FBm);
}

class Chunk{
    public:
        Chunk(int indexX, int indexZ) {
            this->chunkIndexX = indexX;
            this->chunkIndexZ = indexZ;
        }

        void generateChunk() {
            for (int x = 0; x < l; x++)
            {
                for (int z = 0; z < b; z++)
                {
                    float variation = terrainHeight.GetNoise((float)x + (l * chunkIndexX), (float)z + (b * chunkIndexZ));
                    int mapColumnHeight = (int)floor((terrain.GetNoise((float)x + (l * chunkIndexX),(float)z + (b * chunkIndexZ)) + 2) * 5) + variation;
                    for (int y = 0; y < h; y++)
                    {
                        if (y < mapColumnHeight) {
                            this->chunckData[x][z][y] = BLOCKS::BlockState::SOLID;
                        } else {
                            this->chunckData[x][z][y] = BLOCKS::BlockState::AIR;
                        }
                    }
                }
            }
        }

        BLOCKS::BlockState returnBlockState(int x, int y, int z)
        {
            return this->chunckData[x][z][y];
        }

    private:
        int chunkIndexX;
        int chunkIndexZ;
        BLOCKS::BlockState chunckData[l][b][h];
};


