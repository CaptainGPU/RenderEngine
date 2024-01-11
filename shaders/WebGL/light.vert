#version 300 es

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_textcoord;
layout (location = 2) in vec3 a_normal;

out vec3 v_normal;
out vec3 v_position;
out vec3 vertexColor;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform vec3 u_color;
uniform float u_gamma;

void main()
{
    v_normal = mat3(transpose(inverse(u_modelMatrix))) * a_normal;
    v_position = vec3(u_modelMatrix * vec4(a_position, 1.0));

    vertexColor = pow(u_color, vec3(u_gamma));
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position * vec3(0.3), 1.0f);
}