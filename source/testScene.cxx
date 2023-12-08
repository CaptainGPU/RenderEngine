//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "testScene.hxx"
#include "testGameObject.hxx"


TestScene::TestScene()
:Scene("TestScene")
{
}

TestScene::~TestScene()
{
}

void TestScene::construct()
{
    TestGameObject* gameObject = new TestGameObject();
    addGameObject(gameObject);
}


void TestScene::startPlay()
{
    Scene::startPlay();
}
