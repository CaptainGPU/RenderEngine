#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_textcoord;
layout (location = 2) in vec3 a_normal;

out vec3 v_normal;
out vec3 v_position;
out vec3 vertexColor;
out vec4 v_fragPosLightSpace;
out float v_clipSpacePosition;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_sunLightSpaceMatrix;

void main()
{
    v_normal = mat3(transpose(inverse(u_modelMatrix))) * a_normal;
    v_position = vec3(u_modelMatrix * vec4(a_position, 1.0));
    v_fragPosLightSpace = u_sunLightSpaceMatrix * vec4(v_position, 1.0);

    vertexColor = vec3(1.0, .0, .0);
    vec3 color = (a_position + 1.0) * 0.5;
    vertexColor = color;
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0f);
    v_clipSpacePosition = gl_Position.z / 100.0;
}