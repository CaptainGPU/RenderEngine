#version 300 es

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_textcoord;
layout (location = 2) in vec3 a_normal;

out vec3 v_normal;
out vec3 vertexColor;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
void main()
{
    v_normal = mat3(transpose(inverse(u_modelMatrix))) * a_normal;

    vertexColor = vec3(1.0, .0, .0);
    vec3 color = (a_position + 1.0) * 0.5;
    vertexColor = color;
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0f);
}