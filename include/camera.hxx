//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "gameObject.hxx"

#include <glm/glm.hpp>

class Camera : public GameObject
{
public:
	Camera(std::string name);

	glm::mat4 getProjectionMatrix();
	glm::mat4& getViewMatrix();

	virtual void drawGUI() override;
private:
	void calculateCameraVectors();

private:
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;

	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;

	float m_yaw;
	float m_pitch;
	float m_fov;

	bool m_dirtyProjectionMatrix;
	bool m_dirtyViewMatrix;
};