#version 100

precision highp float;

varying vec4 f_color;

void main(void)
{
	gl_FragData[0] = f_color;
}
