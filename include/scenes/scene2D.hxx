//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once


#include "scene.hxx"

#include "vector"

class TileMap;

class Scene2D : public Scene
{
public:
    Scene2D(std::string name);
    ~Scene2D();
    
    void addTileMap(TileMap* map);
    std::vector<TileMap*> getTileMaps();
    
private:
    std::vector<TileMap*> m_tileMaps;
};
