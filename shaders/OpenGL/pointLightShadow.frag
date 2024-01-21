#version 330

uniform vec3 u_lightPosition;
uniform float u_lightRadius;

//precision highp float;
out vec4 color;
in vec3 v_normal;
in vec3 v_position;

void main()
{

    vec3 toLight = v_position - u_lightPosition;
    float distance = length(toLight) / u_lightRadius;

    vec3 finalColor = vec3(distance);

    color = vec4(finalColor, 1.0);
    gl_FragDepth = distance;
}  