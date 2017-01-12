#version 330 core

// in_vs_vertex = vertex data in model space
// vertex data input ALWAYS in layout 0.
layout(location = 0) in vec3 in_vs_vertex;
layout(location = 3) in vec3 in_vs_colors;


// Output data ; will be interpolated for each fragment.
out vec3 fragment_color;

// Values that stay constant for the whole mesh.
uniform mat4 mat4_cam_vp;
uniform mat4 mat4_model;

void main(){
    // Apply all matrix transformations to vert
    gl_Position = mat4_cam_vp * mat4_model * vec4(in_vs_vertex, 1);
	fragment_color = in_vs_colors;
}
