#include "../../include/FastNoiseLite.h"
#include <math.h>
#include <iostream>
#include "block.hpp"

FastNoiseLite terrain;
FastNoiseLite terrainHeight;

int curChunk = 1;

int mapColumnHeight;

const int l = 16;
const int b = 16;
const int h = 2;

namespace BLOCKS{
    enum BlockState {
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

BLOCKS::BlockState returnBlockState(int x, int y, int z, int chunkX, int chunkZ)
{
    float variation = terrainHeight.GetNoise((float)x + (l * chunkX), (float)z + (b * chunkZ));
    mapColumnHeight = (int)floor((terrain.GetNoise((float)x + (l * chunkX),(float)z + (b * chunkZ)) + 1) * 1) + variation;
    if (y <= mapColumnHeight) {
        return BLOCKS::BlockState::SOLID;
    } else {
        return BLOCKS::BlockState::AIR;
    }
}

int height()
{
    return mapColumnHeight;
}

// class Chunk{
//     public:
//         Chunk(int indexX, int indexZ) {
//             this->chunkIndexX = indexX;
//             this->chunkIndexZ = indexZ;
//         }

//         void generateChunk() {
//             for (int x = 0; x < l; x++)
//             {
//                 for (int z = 0; z < b; z++)
//                 {
                    
//                     for (int y = 0; y < h; y++)
//                     {
//                         if (y < mapColumnHeight) {
//                             this->chunckData[x][z][y] = BLOCKS::BlockState::SOLID;
//                         } else {
//                             this->chunckData[x][z][y] = BLOCKS::BlockState::AIR;
//                         }
//                     }
//                 }
//             }
//         }

//     private:
//         int chunkIndexX;
//         int chunkIndexZ;
// };


