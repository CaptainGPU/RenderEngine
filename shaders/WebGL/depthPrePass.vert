#version 300 es

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_textcoord;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in vec3 a_color;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec3 v_worldPos;
out vec3 v_worldNormal;
out vec3 v_vertexColor;

void main()
{
    v_worldPos = vec3(u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0));

    mat3 normalMatrix = transpose(inverse(mat3(u_viewMatrix * u_modelMatrix)));
	v_worldNormal = normalMatrix * a_normal;

    v_vertexColor = a_color;

    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0f);
}