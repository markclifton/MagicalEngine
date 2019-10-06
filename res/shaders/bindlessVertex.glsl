#version 460

#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require

out vec2 UV;
const vec3 vertices[6] = {vec3(-1,-1,0), vec3(1,-1,0), vec3(-1,1,0), vec3(1,-1,0), vec3(1,1,0), vec3(-1,1,0)};
const vec2 uv[6] = {vec2(0,0), vec2(1,0), vec2(0,1), vec2(1,0), vec2(1,1), vec2(0,1)};

void main() {	
    uint id = gl_VertexID;
    UV = uv[id];
    gl_Position = vec4(vertices[id], 1);
}