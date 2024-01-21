#version 330

//precision highp float;
out vec4 color;
in vec3 v_normal;
in vec3 v_position;

void main()
{
    color = vec4(v_position, 1.0);
}  