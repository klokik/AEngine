#version 120

varying vec4 f_color;

void main(void)
{
	gl_FragData[0] = f_color;
}
