#ifndef BUILTIN_H_
#define BUILTIN_H_

#include <stddef.h>


const char *_3vc_v=
{
"#version 100\n"
"\n"
"uniform mat4 u_object_matrix;\n"
"uniform mat4 u_camera_matrix;\n"
"uniform mat4 u_projection_matrix;\n"
"\n"
"uniform vec4 u_color;\n"
"\n"
"attribute vec3 a_vertex;\n"
"attribute vec4 a_color;\n"
"\n"
"varying vec4 f_color;\n"
"\n"
"void main(void)\n"
"{\n"
"	vec4 pos=u_object_matrix*vec4(a_vertex,1.0);\n"
"	gl_Position=u_projection_matrix*u_camera_matrix*pos;\n"
"\n"
"	f_color=a_color*u_color;\n"
"}\n"
""
};

const char *_3vc_f=
{
"#version 100\n"
"\n"
"precision highp float;\n"
"varying vec4 f_color;\n"
"\n"
"void main(void)\n"
"{\n"
"	gl_FragData[0] = f_color;\n"
"}\n"
""
};

#endif