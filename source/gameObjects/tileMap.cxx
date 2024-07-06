//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "gameObjects/tileMap.hxx"

#include "meshLoader.hxx"

#include <cstdlib> // 


TileMap::TileMap(std::string name, 
                 uint32_t tileSize,
                 uint32_t screenWidth,
                 uint32_t screenHeight)
:GameObject(name),
m_tileSize(tileSize)
{
    srand(10);
    Mesh* mesh = createQuad();
    addMesh(mesh);

    m_indexesCount = (25 + 2) * (19 + 2);

    m_indexesInPalletes = new float[m_indexesCount];

    bool b = true;
    for (size_t i = 0; i < m_indexesCount; i++)
    {
        m_indexesInPalletes[i] = rand() % 2;
        b = !b;
    }

    int a = 10;
}

uint32_t TileMap::getTileSize()
{
    return m_tileSize;
}

float* TileMap::getIndexes()
{
    return m_indexesInPalletes;
}

uint32_t TileMap::getIndexesCount()
{
    return m_indexesCount;
}
