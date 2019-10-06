#version 460 core

#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec3 vs_color;
layout (location = 2) in vec3 vs_uv;
layout (location = 3) in mat4 vs_xform;

out vec3 fs_color;
out vec3 fs_uv;

uniform mat4 projection;

void main() {
    gl_Position = projection * vs_xform * vec4(vs_position, 1);
	fs_color = vs_color;
    fs_uv = vs_uv;
}
