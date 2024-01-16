//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "math.hxx"
#include "camera.hxx"
#include "glm/gtx/rotate_vector.hpp"

Frustum::Frustum()
{
	NearTopLeft = glm::vec4(0.0f);
	NearBottomLeft = NearTopLeft;
	NearTopRight = NearTopLeft;
	NearBottomRight = NearTopLeft;
	FarTopLeft = NearTopLeft;
	FarBottomLeft = NearTopLeft;
	FarTopRight = NearTopLeft;
	FarBottomRight = NearTopLeft;
}

void Frustum::calcCorners(Camera* camera)
{
	float AR = 600.0f / 800.0f;
	float tanHalfFOV = glm::tan(glm::radians(camera->getFOV() / 2.0f));

	float NearZ = camera->getNearZ();
	float NearX = NearZ * tanHalfFOV;
	float NearY = NearZ * tanHalfFOV * AR;

	NearTopLeft = glm::vec4(-NearX, NearY, -NearZ, 1.0f);
	NearBottomLeft = glm::vec4(-NearX, -NearY, -NearZ, 1.0f);
	NearTopRight = glm::vec4(NearX, NearY, -NearZ, 1.0f);
	NearBottomRight = glm::vec4(NearX, -NearY, -NearZ, 1.0f);

	float FarZ = camera->getFarZ();;
	float FarX = FarZ * tanHalfFOV;
	float FarY = FarZ * tanHalfFOV * AR;

	FarTopLeft = glm::vec4(-FarX, FarY, -FarZ, 1.0f);
	FarBottomLeft = glm::vec4(-FarX, -FarY, -FarZ, 1.0f);
	FarTopRight = glm::vec4(FarX, FarY, -FarZ, 1.0f);
	FarBottomRight = glm::vec4(FarX, -FarY, -FarZ, 1.0f);
}

void Frustum::calcCorners2(Camera* camera)
{
	float farZ = camera->getFarZ() - camera->getNearZ();
	farZ *= camera->getShadowDistance();
	farZ += camera->getNearZ();
	glm::mat4 proj = glm::perspective(camera->getFOV(), 800.0f / 600.0f, camera->getNearZ(), farZ);

	std::vector<glm::vec4> corners = getFrustumCornersWorldSpace(proj, camera->getViewMatrix());
	NearTopLeft = corners[0];
	NearBottomLeft = corners[1];
	NearTopRight = corners[2];
	NearBottomRight = corners[3];

	FarTopLeft = corners[4];
	FarBottomLeft = corners[5];
	FarTopRight = corners[6];
	FarBottomRight = corners[7];
}

void Frustum::transform(const glm::mat4& m)
{
	NearTopLeft = m * NearTopLeft;
	NearBottomLeft = m * NearBottomLeft;
	NearTopRight = m * NearTopRight;
	NearBottomRight = m * NearBottomRight;

	FarTopLeft = m * FarTopLeft;
	FarBottomLeft = m * FarBottomLeft;
	FarTopRight = m * FarTopRight;
	FarBottomRight = m * FarBottomRight;
}

void Frustum::calcAABB(AABB& aabb)
{
	aabb.add(NearTopLeft);
	aabb.add(NearBottomLeft);
	aabb.add(NearTopRight);
	aabb.add(NearBottomRight);

	aabb.add(FarTopLeft);
	aabb.add(FarBottomLeft);
	aabb.add(FarTopRight);
	aabb.add(FarBottomRight);
}

std::vector<glm::vec4> Frustum::getCorners()
{
	std::vector<glm::vec4> corners;

	corners.push_back(NearTopLeft);
	corners.push_back(NearBottomLeft);
	corners.push_back(NearTopRight);
	corners.push_back(NearBottomRight);

	corners.push_back(FarTopLeft);
	corners.push_back(FarBottomLeft);
	corners.push_back(FarTopRight);
	corners.push_back(FarBottomRight);

	return corners;
}

void AABB::add(const glm::vec3& v)
{
	MinX = glm::min(MinX, v.x);
	MinY = glm::min(MinY, v.y);
	MinZ = glm::min(MinZ, v.z);

	MaxX = glm::max(MaxX, v.x);
	MaxY = glm::max(MaxY, v.y);
	MaxZ = glm::max(MaxZ, v.z);
}

void AABB::updateOrthoInfo(OrthoProjInfo& o)
{
	o.r = MaxX;
	o.l = MinX;
	o.b = MinY;
	o.t = MaxY;
	o.n = MinZ;
	o.f = MaxZ;
}

glm::vec3 AABB::getCenter()
{
	float x = (MinX + MaxX) / 2.0f;
	float y = (MinY + MaxY) / 2.0f;
	float z = (MinZ + MaxZ) / 2.0f;
	return glm::vec3(x, y, z);
}

void calcTightLightProjection(const glm::mat4& CameraView, const glm::vec3& LightDir, Camera* camera, glm::vec3& LightPosWorld, OrthoProjInfo& orthoProjInfo)
{
	//
	// Step #1: Calculate the view frustum corners in view space
	//
	Frustum frustum;
	frustum.calcCorners2(camera);

	//
	// Step #2: transform the view frustum to world space
	//
	//glm::mat4 InverseCameraView = glm::inverse(CameraView);
	//frustum.transform(InverseCameraView);

	Frustum view_frustum_in_world_space = frustum;   // backup for later

	//
	// Step #3: Transform the view frustum to light space (1st time)
	//
	glm::mat4 LightView;
	glm::vec3 Origin(0.0f, 0.0f, 0.0f);
	glm::vec3 Up(0.0f, 1.0f, 0.0f);
	LightView = glm::lookAt(Origin, Origin + LightDir, Up);
	frustum.transform(LightView);

	//
	// Step #4: Calculate an AABB for the view frustum in light space
	//
	AABB aabb;
	frustum.calcAABB(aabb);

	//
	// Step #5: Calculate the position of the light
	//
	glm::vec3 BottomLeft(aabb.MinX, aabb.MinY, aabb.MinZ);
	glm::vec3 TopRight(aabb.MaxX, aabb.MaxY, aabb.MinZ);
	glm::vec4 LightPosWorld4d = glm::vec4((BottomLeft + TopRight) / 2.0f, 1.0f);

	//
	// Step #6: transform the position of the light back to world space
	//
	glm::mat4 LightViewInv = glm::inverse(LightView);
	LightPosWorld4d = LightViewInv * LightPosWorld4d;
	LightPosWorld = glm::vec3(LightPosWorld4d.x, LightPosWorld4d.y, LightPosWorld4d.z);

	//
	// Step #7: transform the view frustum to light space (2nd time)
	//
	LightView = glm::lookAt(LightPosWorld, LightPosWorld + LightDir, Up);
	view_frustum_in_world_space.transform(LightView);

	//
	// Step #8: with the light in its final position recalculate the aabb
	//
	AABB final_aabb;
	view_frustum_in_world_space.calcAABB(final_aabb);
	final_aabb.updateOrthoInfo(orthoProjInfo);
}

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projview)
{
	const auto inv = glm::inverse(projview);

	std::vector<glm::vec4> frustumCorners;
	for (unsigned int x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
				frustumCorners.push_back(pt / pt.w);
			}
		}
	}

	return frustumCorners;
}

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
	return getFrustumCornersWorldSpace(proj * view);
}

glm::mat4 getLightSpaceMatrix(Camera* camera, glm::vec3 lightDir, const float nearPlane, const float farPlane)
{
	const auto proj = glm::perspective(
		glm::radians(camera->getFOV()), (float)800 / (float)600, nearPlane,
		farPlane);
	const auto corners = getFrustumCornersWorldSpace(proj, camera->getViewMatrix());

	glm::vec3 center = glm::vec3(0, 0, 0);
	for (const auto& v : corners)
	{
		center += glm::vec3(v);
	}
	center /= corners.size();

	const auto lightView = glm::lookAt(center, center + lightDir, glm::vec3(0.0f, 1.0f, 0.0f));

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();
	for (const auto& v : corners)
	{
		const auto trf = v;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	// Tune this parameter according to the scene
	constexpr float zMult = 1.0f;
	if (minZ < 0)
	{
		minZ *= zMult;
	}
	else
	{
		minZ /= zMult;
	}
	if (maxZ < 0)
	{
		maxZ /= zMult;
	}
	else
	{
		maxZ *= zMult;
	}

	const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
	return lightProjection * lightView;
}

glm::vec3 getDirectionFromRotation(glm::vec3 rotation)
{
	glm::vec3 forw = glm::vec3(1.0, .0, .0);

	forw = glm::rotate(forw, glm::radians(rotation.z), glm::vec3(0.0, .0, 1.0));
	forw = glm::rotate(forw, glm::radians(rotation.y), glm::vec3(0.0, 1.0, .0));
	forw = glm::rotate(forw, glm::radians(rotation.x), glm::vec3(1.0, .0, .0));

	forw = glm::normalize(forw);

	return forw;
}

glm::mat4 calculateLightProjectionViewMatrix(glm::vec3 lightDirection, Camera* camera)
{
	//lightDirection = glm::vec3(0.0f, 1.0f, 0.0f);

	Frustum frustum;

	frustum.calcCorners2(camera);

	// Step #2: transform the view frustum to world space

	//glm::mat4 inverseCameraView = glm::inverse(camera->getViewMatrix());
	//frustum.transform(inverseCameraView);

	Frustum view_frustum_in_world_space = frustum;

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

	// Step #6: transform the position of the light back to world space

	glm::mat4 lightViewInv = glm::inverse(lightView);
	LightPosWorld4d = lightViewInv * LightPosWorld4d;
	glm::vec3 lightPos = glm::vec3(LightPosWorld4d.x, LightPosWorld4d.y, LightPosWorld4d.z);

	// Step #7: transform the view frustum to light space (2nd time)
	lightView = glm::lookAt(lightPos + glm::vec3(0.001f, 0.0f, 0.0f), lightPos + lightDirection, up);
	view_frustum_in_world_space.transform(lightView);

	OrthoProjInfo orthoProjInfo;

	// Step #8: with the light in its final position recalculate the aabb
	AABB final_aabb;
	view_frustum_in_world_space.calcAABB(final_aabb);
	final_aabb.updateOrthoInfo(orthoProjInfo);

	glm::mat4 lightProj = glm::ortho(orthoProjInfo.l, orthoProjInfo.r, orthoProjInfo.b, orthoProjInfo.t, orthoProjInfo.n, orthoProjInfo.f);

	return lightProj * lightView;
}
