//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "gameObjects/tileMap.hxx"

#include "meshLoader.hxx"


TileMap::TileMap(std::string name, 
                 uint32_t tileSize,
                 uint32_t screenWidth,
                 uint32_t screenHeight)
:GameObject(name),
m_tileSize(tileSize)
{
    Mesh* mesh = createQuad();
    addMesh(mesh);
}

uint32_t TileMap::getTileSize()
{
    return m_tileSize;
}
