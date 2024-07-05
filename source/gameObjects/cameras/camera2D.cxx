//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "gameObjects/cameras/camera2D.hxx"

#include "input.hxx"

Camera2D::Camera2D()
: Camera("DebugCamera", 45.0f, 0.1, 100.0f),
m_speed(100)
{
    
}

void Camera2D::update(float deltaTime)
{
    Camera::update(deltaTime);

    if (!Input::isInputEnable())
    {
        return;
    }

    glm::vec3 front = getFrontVector();
    glm::vec3 right = getRightVector();

    float velocity = deltaTime * m_speed;

    if (Input::keyIsPressed(KEYS::KEY_W))
    {
        addPosition(glm::vec3(.0f, velocity, 0.0f));
    }

    if (Input::keyIsPressed(KEYS::KEY_S))
    {
        addPosition(glm::vec3(.0f, -velocity, 0.0f));
    }

    if (Input::keyIsPressed(KEYS::KEY_A))
    {
        addPosition(glm::vec3( -velocity, 0.0f, 0.0f));
    }

    if (Input::keyIsPressed(KEYS::KEY_D))
    {
        addPosition(glm::vec3( velocity, 0.0f, 0.0f));
    }
}
