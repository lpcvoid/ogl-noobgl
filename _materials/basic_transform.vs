#version 330 core

// in_vs_vertex = vertex data in model space
// vertex data input ALWAYS in layout 0.
in vec3 in_vs_vertex;

// Values that stay constant for the whole mesh.
uniform mat4 mat4_cam_vp;
uniform mat4 mat4_model;

void main(){
    // Apply all matrix transformations to vert
    gl_Position = mat4_cam_vp * mat4_model * vec4(in_vs_vertex, 1);

}
