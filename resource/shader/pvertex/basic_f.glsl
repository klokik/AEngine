#version 100

precision highp float;

uniform vec4 u_color;

uniform sampler2D u_texture0;
uniform int u_istextured0;

varying vec3 f_light;
varying vec3 f_normal;
varying vec3 f_pos;
varying vec2 f_texcoord0;

void setTexture(void);

void main(void)
{
	gl_FragData[0]=u_color;

	if(u_istextured0!=0)
		gl_FragData[0]*=texture2D(u_texture0,f_texcoord0.st);

	gl_FragData[0].rgb*=f_light;
	gl_FragData[0].rgb+=vec3(pow(dot(f_normal,normalize(f_pos)),50.0)*0.8);
}
