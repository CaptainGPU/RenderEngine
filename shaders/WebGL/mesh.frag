#version 300 es

precision highp float;

in vec3 v_normal;
in vec3 v_position;
in vec3 vertexColor;

out vec4 color;
uniform float u_time;
uniform float u_gamma;
uniform vec3 u_albedo;
uniform vec3 u_lightColor;
uniform vec3 u_ambientColor;
uniform float u_smoothness;
uniform float u_ambientStrength;
uniform float u_specularStrength;
uniform vec3 u_cameraPosition;

const vec3 lightPosition = vec3(.0);

void main()
{
    vec3 objectColor = pow(u_albedo, vec3(u_gamma));

    vec3 lightColor = pow(u_lightColor, vec3(u_gamma));
    vec3 ambientColor = pow(u_ambientColor, vec3(u_gamma));

    // ambient
    float ambientStrength = u_ambientStrength;
    vec3 ambient = ambientStrength * ambientColor;
  	
    // diffuse 
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(lightPosition - v_position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = u_specularStrength;
    vec3 viewDir = normalize(u_cameraPosition - v_position);
    vec3 reflectDir = reflect(-lightDir, norm);

    float specPos = mix(2.0, 512.0, u_smoothness);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specPos);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular) * objectColor;

    color = vec4(result, 1.0);
}  