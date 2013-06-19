#version 120

uniform mat4 u_object_matrix;
uniform mat4 u_camera_matrix;
uniform mat4 u_projection_matrix;
uniform int u_shaded;

attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_texcoord0;

varying vec3 f_normal;
varying vec2 f_texcoord0;
varying vec3 f_pos;


void main(void)
{
	vec4 pos=u_object_matrix*vec4(a_vertex,1.0);
	gl_Position=u_projection_matrix*u_camera_matrix*pos;
	
	f_texcoord0=a_texcoord0;
	f_normal=mat3x3(u_camera_matrix*u_object_matrix)*a_normal;

	f_pos = (u_camera_matrix*pos).xyz;
}