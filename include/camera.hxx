//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once

#include "gameObject.hxx"

#include <glm/glm.hpp>

class RenderInfo;
class Uniform;

class Camera : public GameObject
{
public:
	Camera(std::string name, float fov, float nearZ, float farZ);

	glm::mat4& getProjectionMatrix();
	glm::mat4& getViewMatrix();
    glm::mat4& getOrthoMatrix();

	glm::vec3& getFrontVector();
	glm::vec3& getRightVector();

	virtual void drawGUI() override;
    
    void setPitch(const float& pitch);
    void setYaw(const float& yaw);

	float getFOV();
	float getNearZ();
	float getFarZ();

	float getShadowDistance();
	void setShadowDistance(float distance);

	void renderBounds(Uniform* boundColorUniform, RenderInfo& renderInfo);
    
private:
	void calculateCameraVectors();

private:
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
    glm::mat4 m_orthoMatrix;

	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;

	bool m_dirtyProjectionMatrix;
    bool m_dirtOrthoMatrix;
	bool m_dirtyViewMatrix;

protected:
	float m_yaw;
	float m_pitch;
	float m_fov;
	float m_nearZ;
	float m_farZ;
	float m_shadowDistance;
	MeshBound* m_frustumBound;
	MeshBound* m_lightBound;
};
