//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "scenes/scene2D.hxx"

Scene2D::Scene2D(std::string name) :
Scene(name)
{
    
}

Scene2D::~Scene2D()
{ 
}

void Scene2D::addTileMap(TileMap* map)
{
    m_tileMaps.push_back(map);
}

std::vector<TileMap*> Scene2D::getTileMaps()
{
    return m_tileMaps;
}
