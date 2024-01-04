#version 330

//precision highp float;

const float PI = 3.14159265359;

struct PointLight
{
    vec3 position;
    vec3 color;
};

struct SpotLight
{
    vec3 direction;
    vec3 color;
    vec3 position;
    float linear;
    float quadratic;
    float constant;
    float innerCut;
    float outCut;
};

const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

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
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];
uniform int u_pointLightsCount;
uniform int u_spotLightsCount;

vec3 calculatePointLight(int index, vec3 normal, vec3 viewDir, vec3 ambientColor)
{
    vec3 lightColor = u_pointLights[index].color;
    vec3 lightPosition = u_pointLights[index].position;
    vec3 lightDir = normalize(lightPosition - v_position);
    vec3 halfwayDir = normalize(lightDir + viewDir);  

    float diff = max(dot(normal, lightDir), 0.0);

    float specPow = mix(2.0, 512.0, u_smoothness);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), specPow);
    spec *= u_specularStrength;

    float distance = length(lightPosition - v_position);

    float range = 10.0;

    float constant = 1.0f;
    float linear = 4.5 / range;
    float quadratic = 75.0f/(range*range);

    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    attenuation = clamp(attenuation, .0, 1.0);

    vec3 ambient = u_ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(int index, vec3 normal, vec3 viewDir)
{
    vec3 lightPosition = u_spotLights[index].position;
    vec3 lightDir = normalize(lightPosition - v_position);
    vec3 lightDirection = u_spotLights[index].direction;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 lightColor = u_spotLights[index].color;
    vec3 halfwayDir = normalize(lightDir + viewDir);  

    float specPow = mix(2.0, 512.0, u_smoothness);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), specPow);
    spec *= u_specularStrength;

    float distance = length(lightPosition - v_position);

    float constant = u_spotLights[index].constant;
    float linear = u_spotLights[index].linear;
    float quadratic = u_spotLights[index].quadratic;

    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    float theta = dot(lightDir, -lightDirection);

    float cutOff = u_spotLights[index].innerCut;
    float outerCutOff = u_spotLights[index].outCut;

    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = u_ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

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

    for (int i = 0; i < u_spotLightsCount; i++)
    {
        lighing += calculateSpotLight(i, normal, viewDir);
    }

    vec3 finalColor = lighing;
    color = vec4(finalColor, 1.0);
}  