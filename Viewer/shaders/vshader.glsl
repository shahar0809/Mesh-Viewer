#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoords;

out vec2 texCoord;

void main()
{
    gl_Position.xyz = pos;
    gl_Position.z = 0;
    gl_Position.w = 1;
    texCoord = texCoords;
}
