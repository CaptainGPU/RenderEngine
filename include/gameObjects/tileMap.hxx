//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "gameObject.hxx"

class TileMapChunk
{
public:
    TileMapChunk(uint32_t index, int32_t posX, int32_t posY, int32_t width, int32_t height);
    
    bool inChunk(int32_t posX, int32_t posY);
    uint32_t getPalleteIndex(int32_t posX, int32_t posY);
    
private:
    int32_t m_posX;
    int32_t m_posY;
    int32_t m_width;
    int32_t m_height;
    int32_t m_count;
    
    uint32_t* m_palleteIndexes;
};

class TileMap : public GameObject
{
public:
    TileMap(std::string name,
            uint32_t tileSize,
            uint32_t screenWidth,
            uint32_t screenHeight);
    
    virtual void update(float deltaTime);
    
    uint32_t getTileSize();
    float* getIndexes();
    uint32_t getIndexesCount();
    
private:
    uint32_t getTilePalleteIndex(int32_t posX, int32_t posY);
    
    std::vector<TileMapChunk*> m_chunks;
    
private:
    uint32_t m_tileSize;
    float* m_indexesInPalletes;
    uint32_t m_indexesCount;
};
