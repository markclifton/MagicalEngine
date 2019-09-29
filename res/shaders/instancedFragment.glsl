#version 460 core

out vec4 final_color;

in vec3 fs_color;
in vec3 fs_uv;

uniform sampler2D textures[16];

void main() {
    final_color = vec4(fs_color, 1);
    if( fs_uv.z >= 0 ) {
        final_color = texture2D(textures[int(fs_uv.z - .5)], fs_uv.xy);
    }
}
