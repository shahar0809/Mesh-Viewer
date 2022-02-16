#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoords;

// MVP for transformations
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 viewport;
uniform mat4 scaling;

out vec2 texCoord;
out vec4 colorV;

void main()
{
    gl_Position = viewport * projection * view * model * scaling * vec4(pos, 1.0f);
    colorV = vec4(0, 0, 0, 1);
}
