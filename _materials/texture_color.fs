#version 150

uniform vec3 color; // color to blend into texture
uniform sampler2D sampler2d_texture; //this is the texture
in vec2 texture_uv; //this is the texture coord
out vec4 out_fs_finalcolor; //this is the output color of the pixel

void main() {
	out_fs_finalcolor = texture( sampler2d_texture, texture_uv ) * vec4(color, 1.0);
}