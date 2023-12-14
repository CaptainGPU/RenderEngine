//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "input.hxx"
#include <cstdio>

bool Input::m_keys[KEYS::KEY_COUNT];
float Input::m_xChange;
float Input::m_yChange;
bool Input::m_inputEnable;

void Input::init()
{
	for (size_t i = 0; i < KEYS::KEY_COUNT; i++)
	{
		m_keys[i] = false;
	}
	m_xChange = 0.0f;
	m_yChange = 0.0f;
	m_inputEnable = false;
}

bool Input::keyIsPressed(KEYS key)
{
	return m_keys[key];
}

void Input::setKeyPressed(KEYS key, bool isPressed)
{
	m_keys[key] = isPressed;
}

void Input::setXYChange(float x, float y)
{
	m_xChange = x;
	m_yChange = y;
}

float Input::getXChange()
{
	return m_xChange;
}

float Input::getYChange()
{
	return m_yChange;
}

void Input::setInputEnable(bool isEnable)
{
	m_inputEnable = isEnable;
}

bool Input::isInputEnable()
{
	return m_inputEnable;
}
