//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

enum KEYS
{
	KEY_W,
	KEY_S,
	KEY_D,
	KEY_A,
	KEY_Q,
	KEY_E,
	KEY_COUNT = 1024
};

class Input
{
public:
	static void init();
	static bool keyIsPressed(KEYS key);
	static void setKeyPressed(KEYS key, bool isPressed);

private:
	static bool m_keys[KEYS::KEY_COUNT];
};