#version 460 core

out vec4 final_color;

in vec3 fs_color;

void main()
{
    final_color = vec4(fs_color, 1);
}
