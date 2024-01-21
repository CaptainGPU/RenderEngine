#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_textcoord;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec3 v_normal;
out vec3 v_position;

void main()
{
    v_normal = a_normal;
    v_position = vec3(u_modelMatrix * vec4(a_position, 1.0f));
    gl_Position = u_projectionMatrix * u_modelMatrix * vec4(a_position, 1.0f);
}