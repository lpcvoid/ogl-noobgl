#version 330 core

// in_vs_vertex = vertex data in model space
// vertex data input ALWAYS in layout 0.
layout(location = 0) in vec3 in_vs_vertex;

//only this counts. We don't care anymore how it looks like in fragmentshader.
//it uses a different variable, but that's fine, since we just pass this over
layout(location = 1) in vec2 in_vs_uv;

//This is just so we can give stuff to fragment shader.
//see above comment about in_vs_uv.
out vec2 texture_uv;

// Values that stay constant for the whole mesh.
//these are hardcoded into engine - don't change names!
uniform mat4 mat4_cam_vp;
uniform mat4 mat4_model;

void main(){
    // Apply all matrix transformations to vert
    gl_Position = mat4_cam_vp * mat4_model * vec4(in_vs_vertex, 1);

	//pass on the UV coordinates!
	texture_uv = in_vs_uv;
	
}
