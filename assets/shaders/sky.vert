#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 M;
uniform mat4 VP;
uniform vec3 cam_pos;

out vec3 view;

void main()
{
    vec4 world = M * vec4(position, 1.0f);
    view = world.xyz - cam_pos;
    //Since we want the sky to be behind everything (depth=1), we set z to be equal w so after the division, z will be 1.
    gl_Position = (VP * world).xyww;
}