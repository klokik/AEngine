#version 120

uniform vec4 u_color;

uniform sampler2D u_texture0;
uniform int u_istextured0;

varying vec3 f_normal;
varying vec2 f_texcoord0;
varying vec3 f_pos;

void main(void)
{
	gl_FragData[0]=vec4(u_color.rgb,1.0);
	if(u_istextured0!=0)
	{
		gl_FragData[0]*=texture2D(u_texture0,f_texcoord0.st);
	}

	gl_FragData[1] = vec4(f_pos.x,f_pos.y,-f_pos.z,gl_FragCoord.z);
	gl_FragData[2] = vec4(f_normal,1.0);
}