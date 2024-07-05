//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "gameObject.hxx"

class TileMap : public GameObject
{
public:
    TileMap(std::string name,
            uint32_t tileSize,
            uint32_t screenWidth,
            uint32_t screenHeight);
    
    uint32_t getTileSize();
    
private:
    uint32_t m_tileSize;
};
