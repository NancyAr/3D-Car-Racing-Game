#version 330 core

in vec4 vColor;

uniform vec3 tint;

out vec4 color;

void main()
{
    color = vColor * vec4(tint, 1);
}