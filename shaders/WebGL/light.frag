#version 300 es

precision highp float;

in vec3 v_normal;
in vec3 v_position;
in vec3 vertexColor;

out vec4 color;

void main()
{
    color = vec4(vertexColor, 1.0);
}  