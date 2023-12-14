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
	KEY_T,
	KEY_COUNT = 1024
};

class Input
{
public:
	static void init();
	static bool keyIsPressed(KEYS key);
	static void setKeyPressed(KEYS key, bool isPressed);
	static void setXYChange(float x, float y);
	static float getXChange();
	static float getYChange();
	static void setInputEnable(bool isEnable);
	static bool isInputEnable();

private:
	static bool m_keys[KEYS::KEY_COUNT];
	static float m_xChange;
	static float m_yChange;
	static bool m_inputEnable;
};