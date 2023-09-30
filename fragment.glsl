#version 330 core

out vec4 FragColor;

in vec2 UV;

uniform vec3 cam_pos;

const float FOVX = (70.0) / 180 * 3.14159;
const float FOVY = (40.0) / 180 * 3.14159;

const float EPSILON = 0.1;
const float STEP_LIMIT = 255;

const vec4 SKY_COLOR = vec4(0.4, 0.4, 1.0, 1.0);
const vec4 GROUND_COLOR = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 SPHERE_COLOR = vec4(1.0, 0.3, 1.0, 1.0);

const float GROUND = -3.0;

const vec4 SPHERE = vec4(0.0, 1.0, 12.0, 4.0);

float ground_distance(vec3 p) {
    return p.y - GROUND;
}

float sphere_distance(vec3 p, vec4 s) {
    return length(p - s.xyz) - s.w;
}

vec4 raymarch(vec3 ray) {
    float c = 1;
    for (int i = 0; i < STEP_LIMIT; ++i) {
        float gd = ground_distance(ray * c + cam_pos);
        float sd = sphere_distance(ray * c + cam_pos, SPHERE);
        float d = min(gd, sd);

        c += d;

        if (sd <= EPSILON) {
            return SPHERE_COLOR;
        }
        if (gd <= EPSILON) {
            return GROUND_COLOR;
        }
    }

    return SKY_COLOR;
}

void main()
{
    vec3 ray = normalize(vec3(UV.x * 70 / 180, UV.y * 40 / 180, 1.0));

    FragColor = raymarch(ray);
}
