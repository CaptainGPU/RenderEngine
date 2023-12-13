//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "debugCamera.hxx"
#include "input.hxx"

DebugCamera::DebugCamera():
Camera("DebugCamera"),
m_speed(10)
{
}

void DebugCamera::update(float deltaTime)
{
	Camera::update(deltaTime);

	glm::vec3 front = getFrontVector();
	glm::vec3 right = getRightVector();

	float velocity = deltaTime * m_speed;

	if (Input::keyIsPressed(KEYS::KEY_W))
	{
		front *= velocity;
		addPosition(front);
	}

	if (Input::keyIsPressed(KEYS::KEY_S))
	{
		front *= -velocity;
		addPosition(front);
	}

	if (Input::keyIsPressed(KEYS::KEY_A))
	{
		right *= velocity;
		addPosition(-right);
	}

	if (Input::keyIsPressed(KEYS::KEY_D))
	{
		right *= velocity;
		addPosition(right);
	}

	if (Input::keyIsPressed(KEYS::KEY_Q))
	{
		addPosition(glm::vec3(.0f, -velocity, 0.0f));
	}

	if (Input::keyIsPressed(KEYS::KEY_E))
	{
		addPosition(glm::vec3(.0f, velocity, 0.0f));
	}
}
