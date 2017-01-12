#version 330 core

in vec3 fragment_color;

// Ouput data
layout(location = 0) out vec3 out_fs_finalcolor;

void main(){
    out_fs_finalcolor = fragment_color;
}