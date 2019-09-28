#version 460 core

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec3 vs_color;

out vec3 fs_color;

uniform mat4 projection;
uniform mat4 transformation;

void main() {
    gl_Position = /*projection * transformation */ vec4(vs_position, 1);
	fs_color = vs_color;
}
