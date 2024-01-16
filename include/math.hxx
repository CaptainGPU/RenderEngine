//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#pragma once 

#include <glm/glm.hpp>
#include <vector>

class Camera;

struct OrthoProjInfo
{
    float r;        // right
    float l;        // left
    float b;        // bottom
    float t;        // top
    float n;        // z near
    float f;        // z far
};

class AABB
{
public:
    void add(const glm::vec3& v);
    void updateOrthoInfo(OrthoProjInfo& o);
    glm::vec3 getCenter();

public:
    float MinX = FLT_MAX;
    float MaxX = FLT_MIN;
    float MinY = FLT_MAX;
    float MaxY = FLT_MIN;
    float MinZ = FLT_MAX;
    float MaxZ = FLT_MIN;
};

class Frustum
{
public:
    Frustum();
    void calcCorners(Camera* camera);
    void calcCorners2(Camera* camera);
    void transform(const glm::mat4& m);
    void calcAABB(AABB& aabb);
    std::vector<glm::vec4> getCorners();

public:
    glm::vec4 NearTopLeft;
    glm::vec4 NearBottomLeft;
    glm::vec4 NearTopRight;
    glm::vec4 NearBottomRight;

    glm::vec4 FarTopLeft;
    glm::vec4 FarBottomLeft;
    glm::vec4 FarTopRight;
    glm::vec4 FarBottomRight;
};

void calcTightLightProjection(const glm::mat4& CameraView,        // in
    const glm::vec3& LightDir,          // in
    Camera* camera,  // in
    glm::vec3& LightPosWorld,           // out
    OrthoProjInfo& orthoProjInfo);     // out

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projview);
std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
glm::mat4 getLightSpaceMatrix(Camera* camera, glm::vec3 lightDir, const float nearPlane, const float farPlane);

glm::vec3 getDirectionFromRotation(glm::vec3 rotation);

glm::mat4 calculateLightProjectionViewMatrix(glm::vec3 lightDirection, Camera* camera);