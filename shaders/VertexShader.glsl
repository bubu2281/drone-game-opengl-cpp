#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output
out vec3 color;

uniform int object_is_plane;
flat out int is_plane;

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    vec2 u = f*f*(3.0-2.0*f);
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

void main()
{
    vec3 new_position = v_position;
    color = v_color;

    if (object_is_plane == 1) {
        vec4 worldPos = Model * vec4(v_position, 1.0);
        vec3 world_position = worldPos.xyz;

        vec2 st = world_position.xz;
        vec2 pos = vec2(st / 2.0f);
        float n = noise(pos);
        float n_color = noise(pos) * 1.5;

        if (n_color > 1) {
            n_color = 1.0f;
        }

        color = mix(vec3(0, 0.2, 0), vec3(0.3, 0.15, 0), n_color);

        new_position = v_position + vec3(0, n / 2, 0);
    }

    if (object_is_plane == 2) {
        color = vec3(1, 0, 0);
    }
    if (object_is_plane == 3) {
        color = vec3(0, 1, 0);
    }

    is_plane = object_is_plane;
    gl_Position = Projection * View * Model * vec4(new_position, 1.0);
}
