#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_textcoord;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in vec3 a_color;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform float u_cellPalleteIndex[567];

out float v_cellIndex;
out float v_test;

void main()
{
    v_cellIndex = a_textcoord.x;
    int cellIndex = int(v_cellIndex);
    v_test = u_cellPalleteIndex[cellIndex];

    vec3 position = a_position;
    int test = int(v_test);

    if (test == 0)
    {
        position = vec3(0.0);
    }

    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(position, 1.0f);
}