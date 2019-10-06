#version 460 core

#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require

uniform SamplersNV { uint64_t allTheSamplers[1024]; };

out vec4 final_color;

in vec3 fs_color;
in vec3 fs_uv;

void main() {
    final_color = vec4(fs_color, 1);
    if( fs_uv.z >= 0 ) {
        final_color = texture2D(sampler2D(allTheSamplers[int(fs_uv.z)]), fs_uv.xy);
    }
}
