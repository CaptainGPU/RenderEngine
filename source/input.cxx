//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "input.hxx"
#include <cstdio>

bool Input::m_keys[KEYS::KEY_COUNT];

void Input::init()
{
	for (size_t i = 0; i < KEYS::KEY_COUNT; i++)
	{
		m_keys[i] = false;
	}
}

bool Input::keyIsPressed(KEYS key)
{
	return m_keys[key];
}

void Input::setKeyPressed(KEYS key, bool isPressed)
{
	m_keys[key] = isPressed;
}
