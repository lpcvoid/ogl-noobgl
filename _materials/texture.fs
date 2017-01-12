#version 150

uniform sampler2D sampler2d_texture; //this is the texture
in vec2 texture_uv; //this is the texture coord
out vec4 out_fs_finalcolor; //this is the output color of the pixel

void main() {
	out_fs_finalcolor = texture( sampler2d_texture, texture_uv );
}