//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "testScene.hxx"
#include "testGameObject.hxx"
#include "debugCamera.hxx"
#include "pointLightGameObject.hxx"
#include "spotLight.hxx"

#define TWO_PI 6.283

TestScene::TestScene()
:Scene("TestScene"),
m_time(.0f),
m_whiteLight(nullptr)
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
            
            TestGameObject* gameObject = new TestGameObject("monkey_normal.mesh");
            
            gameObject->setPositionX(5.0 * dist - (float(i) * dist));
            //gameObject->setPositionY(5.0 * dist - (float(j) * dist));
            gameObject->setPositionZ(-25.0f - (float(j) * dist));
            
            gameObject->addRotationY((6.28 / 10.0) * count);
            //gameObject->addRotationZ((6.28 / 10.0) * count);
            
            addGameObject(gameObject);
            
            m_meshes.push_back(gameObject);
        }
    }
    
    DebugCamera* camera = new DebugCamera();
    addGameObject(camera);
    setCamera(camera);
    camera->setPosition(glm::vec3(0.0f, 22.0, -5.5f));
    camera->setPitch(-34.65f);
    camera->setYaw(-90.0f);
    
    PoinLightGameObject* pointLight = nullptr;
    
    pointLight = new PoinLightGameObject();
    pointLight->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    addGameObject(pointLight);
    m_lights.push_back(pointLight);

    pointLight = new PoinLightGameObject();
    pointLight->setColor(glm::vec3(.0f, 1.0f, .0f));
    addGameObject(pointLight);
    m_lights.push_back(pointLight);

    pointLight = new PoinLightGameObject();
    pointLight->setColor(glm::vec3(.0f, .0f, 1.0f));
    addGameObject(pointLight);
    m_lights.push_back(pointLight);
    
    m_whiteLight = new SpotLight();
    m_whiteLight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    m_whiteLight->addRotationZ(glm::radians(-45.0f));
    addGameObject(m_whiteLight);
    
    m_plane = new TestGameObject("plane.mesh");
    addGameObject(m_plane);
    
    m_plane->setScale(glm::vec3(20.0));
    m_plane->setPosition(glm::vec3(0.0, -1.0, -40.0));
    
    
    m_spot1 = new SpotLight();
    m_spot1->setColor(glm::vec3(0.0f, .0f, 1.0f));
    m_spot1->addRotationZ(glm::radians(-90.0f));
    m_spot1->setPositionZ(-25.0 - (0.0 * 4.5));
    m_spot1->setPositionY(4.0);
    m_spot1->setPositionX(-2.0 * 4.5);
    
    
    m_spot2 = new SpotLight();
    m_spot2->setColor(glm::vec3(1.0f, .0f, 0.0f));
    m_spot2->addRotationZ(glm::radians(-90.0f));
    m_spot2->setPositionZ(-25.0 - (6.0 * 4.5));
    m_spot2->setPositionY(4.0);
    
    m_spot3 = new SpotLight();
    m_spot3->setColor(glm::vec3(0.0f, 1.0f, .0f));
    m_spot3->setPositionZ(-25.0 - (0.0 * 4.5));
    m_spot3->setPositionY(4.0);
    m_spot3->setPositionX(2.0 * 4.5);
    
    addGameObject(m_spot1);
    addGameObject(m_spot2);
    addGameObject(m_spot3);
}

void TestScene::update(float deltaTime)
{
    m_time += deltaTime;
    
    for (GameObject* gameObject : m_meshes)
    {
        
        //gameObject->addRotationZ(deltaTime);
        gameObject->addRotationY(deltaTime);
    }
    
    int index = 0;
    for (GameObject* gameObject : m_lights)
    {
        float pi = (TWO_PI / m_lights.size()) * index;
        index++;
        
        float sin = glm::sin(m_time + pi);
        float cos = glm::cos(m_time + pi);
        
        float radius = 15;
        
        sin *= radius;
        cos *= radius;
        
        gameObject->setPositionX(sin);
        gameObject->setPositionY(1.0);
        gameObject->setPositionZ(-25.0 - (3.0 * 4.5) + cos);
        
    }
    
    float x = glm::cos(m_time);
    x *= 30;
    
    m_whiteLight->setPositionZ(-25.0 - (3.0 * 4.5));
    m_whiteLight->setPositionY(4.0);
    
    //m_whiteLight->addRotationX(deltaTime);
    m_whiteLight->addRotationY(deltaTime);
    //m_whiteLight->addRotationZ(deltaTime);
    
    m_spot1->addRotationX(deltaTime);
    m_spot2->addRotationZ(deltaTime);
    m_spot3->addRotationZ(-deltaTime);
}


void TestScene::startPlay()
{
    Scene::startPlay();
}
