#version 330

uniform sampler2D u_screenTexture;
uniform float u_chromaticAberration;
uniform float u_sepia;
uniform float u_filmGrain;

in vec2 v_texcoord;

out vec4 color;

// Функція відхилення uv-координат
vec2 getOffsetUV(vec2 center_pos, vec2 uv, float offset)
{
    // Відхилиня від фокусу
    vec2 offsetUV = uv - center_pos;

    // Помножуємо на коєфіцієнт відхилення
    offsetUV *= offset;

    // Вираховуємо відхилення uv-координат
    return uv - offsetUV;
}

// Функція трансформації кольору у сепію
vec3 sepia(vec3 color, float adjust)
{
    // Сила трансформації не може бути меншою за 0, та більше 1
    adjust = clamp(adjust, .0, 1.);

    // Отримуємо значення RGB
    float r = color.r;
    float g = color.g;
    float b = color.b;

    // Коефіцієнти трансформації червоного значення кольору (R)
    float rA = 1. - (.607 * adjust);
    float rB = (.769 * adjust);
    float rC = (.189 * adjust);

    // Коефіцієнти трансформації зеленого значення кольору (G)
    float gA = (.349 * adjust);
    float gB = 1. - (.314 * adjust);
    float gC = (.168 * adjust);

    // Коефіцієнти трансформації синього значення кольору (B)
    float bA = (.272 * adjust);
    float bB = (0.534 * adjust);
    float bC = 1.0 - (0.869 * adjust);

    // Трансформуємо червоне значення кольору (R)
    r = r * rA + g * rB + b * rC;
    // Трансформуємо зелене значення кольору (G)
    g = r * gA + g * gB + b * gC;
    // Трансформуємо синє значення кольору (G)
    b = r * bA + g * bB + b * bC;

    // Значення каналів RGB не може бути менше 0 чи більше 1
    r = clamp(r, .0, 1.);
    g = clamp(g, .0, 1.);
    b = clamp(b, .0, 1.);

    // Повертаємо змінені значення RGB
    return vec3(r, g, b);
}

// Функція обчислення зернистості
float grainFromUV(vec2 uv, float intesity, float anim) 
{
    // Додаємо значення анімації до UV-координати Х.
    uv.x += anim;

    // Число, яке даватиме float-помилку при множенні
    float bigFloat = 493013.0;

    // Обчислення зернистості
    float grain = fract(sin(uv.x + uv.y * 543.31) * bigFloat);
    grain -= .5;

    // Помножуємо зернистість на інтенсивність
    grain *= intesity;
	return grain;
}

void main()
{
    // Позиція фокуса
    vec2 center_pos = vec2(.5 , .5);

    // UV-координати текстури
    vec2 uv = v_texcoord;

    // Отримуємо аберацію (відхилення) для кожного каналу RGB
    vec2 uvRed = getOffsetUV(center_pos, uv, .02 * u_chromaticAberration);
    vec2 uvGreen = getOffsetUV(center_pos, uv, .0 * u_chromaticAberration);
    vec2 uvBlue = getOffsetUV(center_pos, uv, .01 * u_chromaticAberration);

    // Вибираемо RBG значення з текстури
    float r = texture(u_screenTexture, uvRed).r;
    float g = texture(u_screenTexture, uvGreen).g;
    float b = texture(u_screenTexture, uvBlue).b;

    // Хроматична аберація
    vec3 chromaticAberration = vec3(r, g, b);

    // Трансформуємо зображення з хроматичною аберацією в сепію
    // з силою трансформації 80%
    vec3 sepiaColor = sepia(chromaticAberration, u_sepia);

     // Обчислюємо зернистість
    float grain = grainFromUV(uv, u_filmGrain, .0);

    // Додаємо зернистість до зображення
    sepiaColor += grain;

    vec3 finalColor = sepiaColor;
    color = vec4(finalColor, 1.0f);
}