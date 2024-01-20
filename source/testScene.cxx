//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "testScene.hxx"
#include "testGameObject.hxx"
#include "debugCamera.hxx"
#include "pointLightGameObject.hxx"
#include "spotLight.hxx"
#include "sunLight.hxx"

#define TWO_PI 6.283

TestScene::TestScene()
:Scene("TestScene"),
m_time(.0f),
m_whiteLight(nullptr),
m_sun(nullptr)
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
    
    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            count += 1.0;
            
            TestGameObject* gameObject = new TestGameObject("monkey_normal.mesh");
            
            gameObject->setPositionX((i * dist) - (4.5 * dist) );
            gameObject->setPositionZ((j * dist) - (4.5 * dist));
            
            gameObject->addRotationY((360.0f / 10.0) * i * j);
            
            addGameObject(gameObject);
            
            m_meshes.push_back(gameObject);
        }
    }
    
    DebugCamera* camera = new DebugCamera();
    addGameObject(camera);
    setCamera(camera);
    camera->setPosition(glm::vec3(0.0f, 22.0, 30.5f));
    camera->setPitch(-34.65f);
    camera->setYaw(-90.0f);
    
    PoinLightGameObject* pointLight = nullptr;
    
    pointLight = new PoinLightGameObject();
    pointLight->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    addGameObject(pointLight);
    m_lights.push_back(pointLight);

    pointLight = new PoinLightGameObject();
    pointLight->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
    addGameObject(pointLight);
    m_lights.push_back(pointLight);

    pointLight = new PoinLightGameObject();
    pointLight->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
    addGameObject(pointLight);
    m_lights.push_back(pointLight);
    
    m_whiteLight = new SpotLight();
    m_whiteLight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    m_whiteLight->setPosition(glm::vec3(0.0f, 4.0f, 0.0f));
    m_whiteLight->addRotationZ(-45.0f);
    addGameObject(m_whiteLight);
    
    m_plane = new TestGameObject("plane.mesh");
    addGameObject(m_plane);
    
    m_plane->setScale(glm::vec3(50.0));
    m_plane->setPosition(glm::vec3(0.0, -1.0, 0.0));
    
    
    m_spot1 = new SpotLight();
    m_spot1->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    m_spot1->addRotationZ(-89.0f);
    m_spot1->setPositionZ(-10.5);
    m_spot1->setPositionY(4.0f);
    m_spot1->setPositionX(-10.5);
    
    
    m_spot2 = new SpotLight();
    m_spot2->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
    m_spot2->addRotationZ(-89.0f);
    m_spot2->setPositionZ(10.5);
    m_spot2->setPositionY(4.0f);
    
    m_spot3 = new SpotLight();
    m_spot3->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
    m_spot3->setPositionZ(-10.5);
    m_spot3->setPositionY(4.0f);
    m_spot3->setPositionX(10.5);
    
    addGameObject(m_spot1);
    addGameObject(m_spot2);
    addGameObject(m_spot3);

    m_whiteLight->generateBound();
    m_spot1->generateBound();
    m_spot2->generateBound();
    m_spot3->generateBound();
    
    SunLight* sun = new SunLight();
    setSunLight(sun);
    sun->setIntensity(0.0f);
    sun->setColor(glm::vec3(.5f));
    sun->setPositionY(8.0f);
    sun->SetRotateZ(-45.0f);
    //sun->SetRotateY(45.0f);
    addGameObject(sun);
    
    m_sun = sun;
}

void TestScene::update(float deltaTime)
{
    m_time += deltaTime;
    
    for (GameObject* gameObject : m_meshes)
    {
        
        //gameObject->addRotationZ(deltaTime);
        gameObject->addRotationY(deltaTime * 100.0);
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
        gameObject->setPositionZ(cos);
        
    }
    
    float x = glm::cos(m_time);
    x *= 30;
    
    //m_whiteLight->setPositionY(4.0);
    
    //m_whiteLight->addRotationX(deltaTime);
    m_whiteLight->addRotationY(-deltaTime * 90.0f);
    //m_whiteLight  ->addRotationZ(deltaTime);
    
    /*m_spot1->addRotationX(deltaTime * 1.5 * 90.0f);
    m_spot2->addRotationZ(deltaTime * 2.0  * 90.0f);
    m_spot3->addRotationZ(-deltaTime * 2.5  * 90.0f);*/

    float move = glm::cos(m_time);
    m_spot1->setPositionZ(-10.5 * move);
    m_spot2->setPositionX(-10.5 * move);
    m_spot3->setPositionZ( 10.5 * move);

    move = glm::cos(m_time * 2.0f);
    m_spot1->SetRotateX(move * 90.0f);
    m_spot2->SetRotateZ(-90.0f + move * 90.0f);
    m_spot3->SetRotateZ(-90.0f + move * -90.0f);
    
    float sunMove = deltaTime * 45.0f;
   // sunMove = 0.0;
    
    //m_sun->SetRotateZ(-90.0f + sunMove);

    m_sun->addRotationY(sunMove);

    getDefaultCamera()->addRotationY(sunMove);
}


void TestScene::startPlay()
{
    Scene::startPlay();
}
