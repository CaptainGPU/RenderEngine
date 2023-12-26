#version 330

//precision highp float;

struct PointLight
{
    vec3 position;
    vec3 color;
};

const int MAX_POINT_LIGHTS = 4;

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
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
uniform int u_pointLightsCount;

vec3 calculatePointLight(int index, vec3 normal, vec3 viewDir, vec3 ambientColor)
{
    vec3 lightColor = u_pointLights[index].color;
    vec3 lightPosition = u_pointLights[index].position;

    vec3 lightDir = normalize(lightPosition - v_position);

    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);

    float specPos = mix(2.0, 512.0, u_smoothness);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specPos);
    spec *= u_specularStrength;

    float distance = length(lightPosition - v_position);

    float range = 10.0;

    float constant = 1.0f;
    float linear = 4.5 / range;
    float quadratic = 75.0f/(range*range);

    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    
    vec3 ambient = u_ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;


    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 lighing  = vec3(0);

    vec3 normal = normalize(v_normal);
    vec3 viewDir = normalize(u_cameraPosition - v_position);

    vec3 ambientColor = pow(u_ambientColor, vec3(u_gamma));

    for (int i = 0; i < u_pointLightsCount; i++)
    {
        lighing += calculatePointLight(i, normal, viewDir, u_ambientColor);
    }

    vec3 finalColor = lighing;
    color = vec4(finalColor, 0);

    // vec3 objectColor = pow(u_albedo, vec3(u_gamma));
    // //objectColor *= u_pointLights[0].color;

    // vec3 lightPosition = u_pointLights[0].position;

    // vec3 lightColor = pow(u_lightColor, vec3(u_gamma));
    // vec3 ambientColor = pow(u_ambientColor, vec3(u_gamma));

    // // ambient
    // float ambientStrength = u_ambientStrength;
    // vec3 ambient = ambientStrength * ambientColor;
  	
    // // diffuse 
    // vec3 norm = normalize(v_normal);
    // vec3 lightDir = normalize(lightPosition - v_position);
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = diff * lightColor;

    // // specular
    // float specularStrength = u_specularStrength;
    // vec3 viewDir = normalize(u_cameraPosition - v_position);
    // vec3 reflectDir = reflect(-lightDir, norm);

    // float specPos = mix(2.0, 512.0, u_smoothness);

    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), specPos);
    // vec3 specular = specularStrength * spec * lightColor;  

    // vec3 result = (ambient + diffuse + specular) * objectColor;

    // color = vec4(result, 1.0);
}  