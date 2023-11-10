#include <iostream>

enum Block {
    GRASS,
    DIRT,
    STONE
};

const int blocks = 3;

int blockTextureMap[blocks][6];

const char* blocktextures[] = {
    "GrassBlock_TOP.jpg",
    "GrassBlock_SIDE.jpg",
    "DirtBlock.jpg",
    "StoneBlock.jpg"
};

void initBlockTextures()
{
    blockTextureMap[Block::GRASS][0] = 1;
    blockTextureMap[Block::GRASS][1] = 2;
    blockTextureMap[Block::GRASS][2] = 2;
    blockTextureMap[Block::GRASS][3] = 2;
    blockTextureMap[Block::GRASS][4] = 2;
    blockTextureMap[Block::GRASS][5] = 3;
    blockTextureMap[Block::GRASS][0] = 4;
}