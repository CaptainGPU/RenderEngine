#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_textcoord;

out vec2 v_texcoord;

void main()
{
    v_texcoord = a_textcoord;
    gl_Position = vec4(a_position, 1.0);
}