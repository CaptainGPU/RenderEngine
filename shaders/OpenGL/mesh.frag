#version 330

precision highp float;

in vec3 v_normal;
in vec3 vertexColor;

out vec4 color;
uniform float u_time;

// Позиція камери
vec3 cameraPosition = vec3(.0, 0, 1.0);
// Позиція джерела світла
vec3 lightPosition = vec3(1.0, 1.0, 1.0);

vec3 warmColor = vec3(.4, .4, .0); // "Теплий колір"
vec3 coolColor = vec3(.0, .0, .4); // "Холодний колір"
float warm = 1.0; // Коефіцієнт "Теплого кольору"
float cool = 1.0; // Коефіцієнт "Холодного кольору"
vec3 albedo = vec3(.4, .0, .0); // Альбедо-колір моделі
float specularCoef = 0.3; // Коефіцієнт дзеркального відблиску

void main()
{
    // фінальний колір пикселя, обчислений у шейдері
    vec3 finalColor = vec3(.0);

    // Нормалізуємо напрямок до джерела світла
    vec3 lightDirection = normalize(lightPosition);
    // Нормалізуємо нормаль поверхні
    vec3 nNormal = normalize(v_normal.xyz);
    // Нормалізуємо напрямок до камери
    vec3 viewDirection = normalize(cameraPosition);

    // Отримуємо "теплий колір" градіенту
    vec3 kWarm = warmColor + albedo * warm;
    
    // Отримуємо "холодний колір" градіенту
    vec3 kCool = coolColor + albedo * cool;

    // Обчислюємо alpha-значення градієнта
    float alpha = (1.0 + dot(lightDirection, nNormal)) * 0.5;
    // Альтернативне (дифузне) alpha-значення градієнта
    // alpha = dot(lightDirection, nNormal);

    // Обчислюємо gooch-градієнт
    vec3 gooch = (alpha * kWarm) + ((1.0 - alpha) * kCool);

    // Отримуємо віддзеркалений від поверхні моделі вектор 
    // від джерела світла
    vec3 reflection = normalize(reflect(-lightDirection, nNormal));
    
    // Обчислюємо значення specular освітлення,
    // використовуючи скалярне множення вектора камери 
    // на дзеркально відбитий вектор від джерела світла
    float specular = max(dot(reflection, viewDirection), .0);

    // Робимо відблиск більш глянцевим, зводячи значення дзеркального
    // освітлення в ступінь 128
    specular = pow(specular, 128.0);

    // Помножуємо значення дзеркального освітлення на коефіцієнт
    specular *= specularCoef;

    // Фінальний колір пікселя = 
    // gooch + значення дзеркального освітлення
    finalColor = gooch + specular;

    color = vec4(finalColor, 1.0);
}  