#version 100

uniform mat4 u_object_matrix;
uniform mat4 u_camera_matrix;
uniform mat4 u_projection_matrix;

uniform vec4 u_color;

attribute vec3 a_vertex;
attribute vec4 a_color;

varying vec4 f_color;

void main(void)
{
	vec4 pos=u_object_matrix*vec4(a_vertex,1);
	gl_Position=u_projection_matrix*u_camera_matrix*pos;

	f_color=a_color*u_color;
}
