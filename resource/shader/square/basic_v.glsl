#version 120

uniform vec2 u_size;

attribute vec3 a_position;

varying vec2 f_pos;
varying vec2 f_offset;

void main(void)
{
	f_offset=vec2(1.0/u_size.x,1.0/u_size.y);
	f_pos=a_position.xy+vec2(0.5);
	gl_Position=vec4(a_position.xyz*2,1.0);
}