//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "gameObjects/tileMap.hxx"

#include "meshLoader.hxx"

#include "engine.hxx"
#include "scenes/scene2D.hxx"
#include "gameObjects/cameras/camera2D.hxx"

#include <cstdlib> // 


TileMapChunk::TileMapChunk(uint32_t index, int32_t posX, int32_t posY, int32_t width, int32_t height) :
m_width(width),
m_height(height),
m_posX(posX),
m_posY(posY)
{
    srand(index);
    
    m_count = width * height;
    m_palleteIndexes = new uint32_t[m_count];
    
    for (size_t i = 0; i < m_count; i++)
    {
        m_palleteIndexes[i] = rand() % 2;
    }
}

uint32_t TileMapChunk::getPalleteIndex(int32_t posX, int32_t posY)
{
    int32_t lX = posX - m_posX;
    int32_t lY = posY - m_posY;
    
    int32_t index = lX * m_width + lY;
    
    return m_palleteIndexes[index];
}

bool TileMapChunk::inChunk(int32_t posX, int32_t posY)
{
    if (posX < m_posX)
    {
        return false;
    }
    
    if (posX >= m_posX + m_width)
    {
        return false;
    }
    
    if (posY < m_posY)
    {
        return false;
    }
    
    if (posY >= m_posY + m_height)
    {
        return false;
    }
    
    return true;
}


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
    
    uint32_t index = 0;
    for (int i = -10; i < 10; i++)
    {
        for (int j = -10; j < 10; j++)
        {
            TileMapChunk* chunk = new TileMapChunk(index, i * 10, j * 10, 10, 10);
            m_chunks.push_back(chunk);
            index++;
        }
    }

}

void TileMap:: update(float deltaTime)
{
    GameObject::update(deltaTime);
    
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene2D* scene = dynamic_cast<Scene2D*>(manager->getScene());
    
    if (!scene)
    {
        return;
    }
    
    Camera* camera = scene->getCamera();
    
    glm::vec3 cameraPos = camera->getPosition();
    
    int cameraX = floor((float)cameraPos.x / m_tileSize);
    int cameraY = floor((float)cameraPos.y / m_tileSize);
    
    if (cameraX < 0)
    {
        cameraX++;
    }
    
    if (cameraY < 0)
    {
        cameraY++;
    }
    
    int cellIndex = 0;
    for (int pX = cameraX-1; pX < cameraX+26; pX++)
    {
        for (int pY = cameraY-1; pY < cameraY+20; pY++)
        {
            float palleteIndex= (float)getTilePalleteIndex(pX, pY);
            m_indexesInPalletes[cellIndex] = palleteIndex;
            cellIndex++;
        }
    }
    
    int a = 10;
}

uint32_t TileMap::getTilePalleteIndex(int32_t posX, int32_t posY)
{
    for (TileMapChunk* chunk : m_chunks)
    {
        if (chunk->inChunk(posX, posY))
        {
            return chunk->getPalleteIndex(posX, posY);
        }
    }
    
    return 0;
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
