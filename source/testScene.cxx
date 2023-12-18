//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "testScene.hxx"
#include "testGameObject.hxx"
#include "debugCamera.hxx"


TestScene::TestScene()
:Scene("TestScene")
{
}

TestScene::~TestScene()
{
}

void TestScene::construct()
{
    Scene::construct();

    float dist = 3.0;
    
    float count = .0;
    
    for (size_t i = 0; i < 11; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            count += 1.0;
            
            TestGameObject* gameObject = new TestGameObject();
            
            gameObject->setPositionX(5.0 * dist - (float(i) * dist));
            gameObject->setPositionY(5.0 * dist - (float(j) * dist));
            gameObject->setPositionZ(-25.0f);
            
            gameObject->addRotationY((6.28 / 10.0) * count);
            //gameObject->addRotationZ((6.28 / 10.0) * count);
            
            addGameObject(gameObject);
        }
    }
    
    DebugCamera* camera = new DebugCamera();
    addGameObject(camera);
    setCamera(camera);
}

void TestScene::update(float deltaTime)
{
    size_t numChilds = getGameObjectCount();
    for (size_t i = 0; i < numChilds; i++)
    {
        GameObject* gameObject = getGameObject(i);
        
        //gameObject->addRotationZ(deltaTime);
        gameObject->addRotationY(deltaTime);
    }
}


void TestScene::startPlay()
{
    Scene::startPlay();
}
