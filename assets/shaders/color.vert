#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 M;
uniform mat4 VP;
uniform vec3 offset;
uniform vec4 color;

out vec2 TexCoords;
out vec4 vColor;

void main()
{
    
    float scale = 10.0f;
    vColor = color;
    gl_Position = VP * M * vec4(position, 1.0f);
    //gl_Position = projection * vec4((vertex.x, vertex.y * scale) + offset, 0.0, 1.0);
}