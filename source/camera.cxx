//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "camera.hxx"
#include "sceneManager.hxx"
#include "math.hxx"
#include "meshLoader.hxx"
#include "render.hxx"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

Camera::Camera(std::string name, float fov, float nearZ, float farZ):
GameObject(name),
m_projectionMatrix(glm::mat4(1.0f)),
m_viewMatrix(glm::mat4(1.0f)),
m_dirtyProjectionMatrix(true),
m_dirtyViewMatrix(true),
m_front(.0f),
m_right(.0f),
m_up(.0f),
m_yaw(0.0f),
m_pitch(.0f),
m_fov(fov),
m_nearZ(nearZ),
m_farZ(farZ),
m_shadowDistance(0.5f)
{
	//setPositionX(20.0f);

	/*
	std::vector<glm::vec4> corners = getFrustumCornersWorldSpace(getProjectionMatrix(), getViewMatrix());
	m_frustumBound = createCorterBound(corners);

	glm::vec3 dir = glm::vec3(1.0, 0.0, 0.0);

	glm::mat4 lightMatrix = calculateLightProjectionViewMatrix(dir, this);

	glm::mat4 lP = glm::ortho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
	glm::mat4 lV = glm::lookAt(glm::vec3(10.0, 0.0, 0.0), glm::vec3(11.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	corners = getFrustumCornersWorldSpace(lP * lV);
	m_lightBound = createCorterBound(corners);


	glm::vec3 camRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 camDirection = getDirectionFromRotation(camRotation);
	glm::mat4 cameraViewMatrix = glm::lookAt(glm::vec3(0.0), camDirection, glm::vec3(0.0, 1.0, 0.0));

	glm::vec3 lightRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 lightDirection = getDirectionFromRotation(lightRotation);
	glm::mat4 lightViewMatrix = glm::lookAt(glm::vec3(0.0), lightDirection, glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 orthoProj = glm::ortho(-10.0, 10.0, -1.0, 1.0, -2.0, 2.0);

	// Step #1: Calculate the view frustum corners in view space
	Frustum frustum;

	frustum.calcCorners(this);

	// Step #2: transform the view frustum to world space

	glm::mat4 inverseCameraView = glm::inverse(cameraViewMatrix);
	frustum.transform(inverseCameraView);

	Frustum view_frustum_in_world_space = frustum;

	std::vector<glm::vec4> corners = getFrustumCornersWorldSpace(getProjectionMatrix(), cameraViewMatrix);
	m_frustumBound = createCorterBound(frustum.getCorners());

	// Step #3: Transform the view frustum to light space (1st time)

	glm::mat4 lightView;
	glm::vec3 origin = glm::vec3(0.0);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	lightView = glm::lookAt(origin, lightDirection, up);
	frustum.transform(lightView);

	// Step #4: Calculate an AABB for the view frustum in light space
	AABB aabb;
	frustum.calcAABB(aabb);

	// Step #5: Calculate the position of the light

	glm::vec3 BottomLeft(aabb.MinX, aabb.MinY, aabb.MinZ);
	glm::vec3 TopRight(aabb.MaxX, aabb.MaxY, aabb.MaxZ);
	glm::vec4 LightPosWorld4d = glm::vec4((BottomLeft + TopRight) / 2.0f, 1.0f);
	LightPosWorld4d = glm::vec4(aabb.getCenter(), 1.0f);

	// Step #6: transform the position of the light back to world space
	
	glm::mat4 lightViewInv = glm::inverse(lightView);
	LightPosWorld4d = lightViewInv * LightPosWorld4d;
	glm::vec3 lightPos = glm::vec3(LightPosWorld4d.x, LightPosWorld4d.y, LightPosWorld4d.z);

	// Step #7: transform the view frustum to light space (2nd time)
	lightView = glm::lookAt(lightPos, lightPos + lightDirection, up);
	view_frustum_in_world_space.transform(lightView);

	OrthoProjInfo orthoProjInfo;

	// Step #8: with the light in its final position recalculate the aabb
	AABB final_aabb;
	view_frustum_in_world_space.calcAABB(final_aabb);
	final_aabb.updateOrthoInfo(orthoProjInfo);

	glm::mat4 lightProj = glm::ortho(orthoProjInfo.l, orthoProjInfo.r, orthoProjInfo.b, orthoProjInfo.t, orthoProjInfo.n, orthoProjInfo.f);

	std::vector<glm::vec4> lightCorners = getFrustumCornersWorldSpace(lightProj, lightView);

	m_lightBound = createCorterBound(lightCorners);*/

	/*AABB aabb;
	glm::vec3 center = glm::vec3(0, 0, 0);
	for (const auto& v : corners)
	{
		center += glm::vec3(v);
		aabb.add(v);
	}
	//
	//center = glm::vec3(0.0);
	//center /= corners.size();
	center = aabb.getCenter();

	glm::mat4 orthoProj = glm::ortho(-10.0, 10.0, -1.0, 1.0, -2.0, 2.0);
	glm::mat4 lightViewFromFrustumCenter = glm::lookAt(center, center + lightDirection, glm::vec3(0.0, 1.0, 0.0));

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();
	for (const auto& v : corners)
	{
		const auto trf = lightViewFromFrustumCenter * v;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

	std::vector<glm::vec4> cornersOrtho = getFrustumCornersWorldSpace(lightProjection, lightViewFromFrustumCenter);
	m_lightBound = createCorterBound(cornersOrtho);*/
}

glm::mat4& Camera::getProjectionMatrix()
{
	if (m_dirtyProjectionMatrix)
	{
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), float(800.0f / 600.0f), m_nearZ, m_farZ);
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

	ImGui::SliderFloat("FOV", &fov, 1.0f, 180.0f);
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

void Camera::setPitch(const float& pitch)
{
    m_pitch = pitch;
}

void Camera::setYaw(const float& yaw)
{
    m_yaw = yaw;
}

float Camera::getFOV()
{
	return m_fov;
}

float Camera::getNearZ()
{
	return m_nearZ;
}

float Camera::getFarZ()
{
	return m_farZ;
}

float Camera::getShadowDistance()
{
	return m_shadowDistance;
}

void Camera::setShadowDistance(float distance)
{
	m_shadowDistance = distance;
}

void Camera::renderBounds(Uniform* boundColorUniform, RenderInfo& renderInfo)
{
	glm::vec3 color = glm::vec3(0.0, 1.0, 1.0);
	boundColorUniform->setVec3(color);

	Render::drawMeshBound(m_frustumBound, renderInfo);
	
	color = glm::vec3(1.0f, 0.0f, 0.0f);
	boundColorUniform->setVec3(color);

	Render::drawMeshBound(m_lightBound, renderInfo);
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
