#version 330 core

in vec3 view;

uniform vec3 sun_direction;
uniform float sun_size;
uniform float sun_halo_size;
uniform float sun_brightness;
uniform vec3 sun_color;
uniform vec3 sky_top_color;
uniform vec3 sky_bottom_color;
uniform float sky_smoothness;

out vec4 color;

void main()
{
    vec3 dir = normalize(view);
    vec3 sky_color = mix(sky_bottom_color, sky_top_color, smoothstep(-sky_smoothness, sky_smoothness, dir.y));
    float sun = smoothstep(1-sun_size-sun_halo_size,1-sun_size, dot(sun_direction, dir));
    color = vec4(sky_color + sun_brightness * sun_color * sun, 1.0f);
}