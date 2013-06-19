#version 120

uniform mat4 u_object_matrix;
uniform mat4 u_camera_matrix;
uniform mat4 u_projection_matrix;

attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_texcoord0;

varying vec3 f_normal;
varying vec2 f_texcoord0;
varying vec3 f_pos;
varying vec3 f_eyevec;

//forward declaration
void passData(void);

void main(void)
{
	vec4 pos=u_object_matrix*vec4(a_vertex,1.0);
	gl_Position=u_projection_matrix*u_camera_matrix*pos;
	
	f_eyevec=pos.xyz;

	passData(void);
}

void passData(void)
{
	f_texcoord0=a_texcoord0;
	f_normal=gl_NormalMatrix*a_normal;
	f_pos=f_eyevec;//a_vertex;
	//f_eyevec=gl_Position.xyz;
}
