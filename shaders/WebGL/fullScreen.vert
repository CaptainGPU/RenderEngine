#version 300 es

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_textcoord;

void main()
{
    gl_Position = vec4(a_position, .0, 1.);
}