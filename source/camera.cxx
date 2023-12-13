//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "camera.hxx"
#include "sceneManager.hxx"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

Camera::Camera(std::string name):
GameObject(name),
m_projectionMatrix(glm::mat4(1.0f)),
m_viewMatrix(glm::mat4(1.0f)),
m_dirtyProjectionMatrix(true),
m_dirtyViewMatrix(true),
m_front(.0f),
m_right(.0f),
m_up(.0f),
m_yaw(-90.0f),
m_pitch(.0f),
m_fov(45.0f)
{

}

glm::mat4& Camera::getProjectionMatrix()
{
	if (m_dirtyProjectionMatrix)
	{
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), float(800.0f / 600.0f), 0.1f, 100.0f);
		m_dirtyProjectionMatrix = false;
	}

	return m_projectionMatrix;
}

glm::mat4& Camera::getViewMatrix()
{
	calculateCameraVectors();

	glm::vec3 position = getPosition();
	
	m_viewMatrix = glm::lookAt(position, position + m_front, m_up);

	return m_viewMatrix;
}

glm::vec3& Camera::getFrontVector()
{
	return m_front;
}

glm::vec3& Camera::getRightVector()
{
	return m_right;
}

void Camera::drawGUI()
{
	/*GameObject::drawGUI();

	ImGui::Begin("Debug Camera");

	float yaw = m_yaw;
	float pitch = m_pitch;
	float fov = m_fov;

	ImGui::SliderFloat("fow", &fov, 1.0f, 180.0f);
	ImGui::SliderFloat("yaw", &yaw, -180.0f, 180.0f);
	ImGui::SliderFloat("pitch", &pitch, -90.0f, 90.0f);

	m_pitch = pitch;
	m_yaw = yaw;
	if (m_fov != fov)
	{
		m_fov = fov;
		m_dirtyProjectionMatrix = true;
	}

	ImGui::End();*/
}

void Camera::calculateCameraVectors()
{
	glm::vec3 front;
	glm::vec3 WorldUp = glm::vec3(.0f, 1.0f, .0f);

	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, WorldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
