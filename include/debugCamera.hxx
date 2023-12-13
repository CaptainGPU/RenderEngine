//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "camera.hxx"

class DebugCamera : public Camera
{
public:
	DebugCamera();

	virtual void update(float deltaTime) override;

private:
	float m_speed;
};