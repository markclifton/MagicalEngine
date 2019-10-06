#version 460

#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require

out vec4 fragColor;
in vec2 UV;
uniform SamplersNV { uint64_t allTheSamplers[1]; };

void main () {
    fragColor = vec4(texture2D(sampler2D(allTheSamplers[0]), UV).rgb, 1.0);
}