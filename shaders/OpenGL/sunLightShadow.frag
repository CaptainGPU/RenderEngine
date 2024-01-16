#version 330

//precision highp float;
out vec4 color;
in vec3 v_normal;

void main()
{
    color = vec4(v_normal, 1.0);
}  