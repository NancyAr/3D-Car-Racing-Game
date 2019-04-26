#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 normal;

uniform mat4 M;
uniform mat4 M_it;

uniform mat4 VP;
uniform vec3 cam_pos;

out Interpolators {
    vec2 uv;
    vec3 view;
    vec3 normal;
} vs_out;

void main()
{
    vs_out.uv = uv;
    vec4 world = M * vec4(position, 1.0f);
    vs_out.view = cam_pos - world.xyz;
    vs_out.normal = normalize(M_it * vec4(normal, 0.0f)).xyz;
    gl_Position = VP * world;
}