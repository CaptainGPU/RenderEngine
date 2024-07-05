#version 330

in vec3 v_worldPos;
in vec3 v_worldNormal;
in vec3 v_vertexColor;

layout (location = 0) out vec4 gColor;

float farPlane = 100.0f;
float nearPlane = 0.1f;

float linearDepth(float depth)
{
	float z = depth * 2.0f - 1.0f; 
	return (2.0f * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));	
}

void main()
{
    //gPosition = vec4(v_worldPos, linearDepth(gl_FragCoord.z));
    //gNormal = vec4(normalize(v_worldNormal) * 0.5 + 0.5, 1.0);
    gColor = vec4(v_vertexColor, 1.0);
}