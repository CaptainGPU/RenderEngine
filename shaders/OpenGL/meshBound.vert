#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_textcoord;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform vec3 u_boundColor;

out vec3 boundColor;

void main()
{
    boundColor = u_boundColor;
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0f);
}