#pragma once

#include "../../include/glad/glad.h"

#include <math.h>
#include <iostream>
#include <vector>

enum BlockState
{
    AIR,
    SOLID,
    LIQUID
};

enum Block
{
    EMPTY,
    GRASS_BLOCK,
    GRASS,
    DIRT,
    STONE,
    SAND,
    WATER
};

enum BlockFace
{
    BACK,
    FRONT,
    SIDE_LEFT,
    SIDE_RIGHT,
    BOTTOM,
    TOP,
    PLAIN
};

struct block_mesh
{
    Block name;
    glm::vec3 pos;
    std::vector<BlockFace> faces;

    block_mesh(Block b_name, glm::vec3 b_pos)
    {
        name = b_name;
        pos = b_pos;
    }
};

class block_data
{
    public:
        Block name;
        BlockState type;
        int blockTextureMap[7];

    block_data(Block n, BlockState t)
    {
        name = n;
        type = t;
    }
};

block_data* blocks[5];

void init_blocks()
{
    //AIR
    blocks[Block::EMPTY] = new block_data(Block::EMPTY, BlockState::AIR);

    // Grass
    blocks[Block::GRASS_BLOCK] = new block_data(Block::GRASS_BLOCK, BlockState::SOLID);
    blocks[Block::GRASS_BLOCK]->blockTextureMap[BlockFace::FRONT] = 2;
    blocks[Block::GRASS_BLOCK]->blockTextureMap[BlockFace::BACK] = 2;
    blocks[Block::GRASS_BLOCK]->blockTextureMap[BlockFace::SIDE_RIGHT] = 2;
    blocks[Block::GRASS_BLOCK]->blockTextureMap[BlockFace::SIDE_LEFT] = 2;
    blocks[Block::GRASS_BLOCK]->blockTextureMap[BlockFace::TOP] = 1;
    blocks[Block::GRASS_BLOCK]->blockTextureMap[BlockFace::BOTTOM] = 3;

    //Stone
    blocks[Block::STONE] = new block_data(Block::STONE, BlockState::SOLID);
    blocks[Block::STONE]->blockTextureMap[BlockFace::FRONT] = 4;
    blocks[Block::STONE]->blockTextureMap[BlockFace::BACK] = 4;
    blocks[Block::STONE]->blockTextureMap[BlockFace::SIDE_RIGHT] = 4;
    blocks[Block::STONE]->blockTextureMap[BlockFace::SIDE_LEFT] = 4;
    blocks[Block::STONE]->blockTextureMap[BlockFace::TOP] = 4;
    blocks[Block::STONE]->blockTextureMap[BlockFace::BOTTOM] = 4;

    //WATER
    blocks[Block::WATER] = new block_data(Block::WATER, BlockState::LIQUID);
    blocks[Block::WATER]->blockTextureMap[BlockFace::FRONT] = 240;
    blocks[Block::WATER]->blockTextureMap[BlockFace::BACK] = 240;
    blocks[Block::WATER]->blockTextureMap[BlockFace::SIDE_RIGHT] = 240;
    blocks[Block::WATER]->blockTextureMap[BlockFace::SIDE_LEFT] = 240;
    blocks[Block::WATER]->blockTextureMap[BlockFace::TOP] = 240;
    blocks[Block::WATER]->blockTextureMap[BlockFace::BOTTOM] = 240;

    //DIRT
    blocks[Block::DIRT] = new block_data(Block::DIRT, BlockState::SOLID);
    blocks[Block::DIRT]->blockTextureMap[BlockFace::FRONT] = 3;
    blocks[Block::DIRT]->blockTextureMap[BlockFace::BACK] = 3;
    blocks[Block::DIRT]->blockTextureMap[BlockFace::SIDE_RIGHT] = 3;
    blocks[Block::DIRT]->blockTextureMap[BlockFace::SIDE_LEFT] = 3;
    blocks[Block::DIRT]->blockTextureMap[BlockFace::TOP] = 3;
    blocks[Block::DIRT]->blockTextureMap[BlockFace::BOTTOM] = 3;

    //SAND
    blocks[Block::SAND] = new block_data(Block::SAND, BlockState::SOLID);
    blocks[Block::SAND]->blockTextureMap[BlockFace::FRONT] = 17;
    blocks[Block::SAND]->blockTextureMap[BlockFace::BACK] = 17;
    blocks[Block::SAND]->blockTextureMap[BlockFace::SIDE_RIGHT] = 17;
    blocks[Block::SAND]->blockTextureMap[BlockFace::SIDE_LEFT] = 17;
    blocks[Block::SAND]->blockTextureMap[BlockFace::TOP] = 17;
    blocks[Block::SAND]->blockTextureMap[BlockFace::BOTTOM] = 17;

    //SAND
    blocks[Block::GRASS] = new block_data(Block::GRASS, BlockState::SOLID);
    blocks[Block::GRASS]->blockTextureMap[BlockFace::PLAIN] = 51;
}