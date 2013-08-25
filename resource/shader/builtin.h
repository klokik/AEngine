#ifndef BUILTIN_H_
#define BUILTIN_H_

#include <stddef.h>

#define _3vc_v \
"#version 100\n" \
"\n" \
"uniform mat4 u_object_matrix;\n" \
"uniform mat4 u_camera_matrix;\n" \
"uniform mat4 u_projection_matrix;\n" \
"\n" \
"uniform vec4 u_color;\n" \
"\n" \
"attribute vec3 a_vertex;\n" \
"attribute vec4 a_color;\n" \
"\n" \
"varying vec4 f_color;\n" \
"\n" \
"void main(void)\n" \
"{\n" \
"	vec4 pos=u_object_matrix*vec4(a_vertex,1.0);\n" \
"	gl_Position=u_projection_matrix*u_camera_matrix*pos;\n" \
"\n" \
"	f_color=a_color*u_color;\n" \
"}\n"

#define _3vc_f \
"#version 100\n" \
"\n" \
"precision highp float;\n" \
"varying vec4 f_color;\n" \
"\n" \
"void main(void)\n" \
"{\n" \
"	gl_FragData[0] = f_color;\n" \
"}\n" \
""

#define _3vmn_v \
"#version 100\n" \
"\n" \
"uniform mat4 u_object_matrix;\n" \
"uniform mat4 u_camera_matrix;\n" \
"uniform mat4 u_projection_matrix;\n" \
"\n" \
"\n" \
"attribute vec3 a_vertex;\n" \
"attribute vec3 a_normal;\n" \
"attribute vec2 a_texcoord0;\n" \
"\n" \
"\n" \
"varying vec2 f_texcoord0;\n" \
"varying vec3 f_normal;\n" \
"\n" \
"void main(void)\n" \
"{\n" \
"	vec4 pos=u_object_matrix*vec4(a_vertex,1.0);\n" \
"	gl_Position=u_projection_matrix*u_camera_matrix*pos;\n" \
"\n" \
"	f_normal=(u_object_matrix*vec4(a_normal,0.0)).xyz;\n" \
"	f_texcoord0=a_texcoord0;" \
"}\n"

#define _3vmn_f \
"#version 100\n" \
"\n" \
"precision highp float;\n" \
"\n" \
"uniform sampler2D u_texture0;\n" \
"uniform int u_istextured0;\n" \
"uniform vec4 u_color;\n" \
"\n" \
"varying vec3 f_normal;\n" \
"varying vec2 f_texcoord0;\n" \
"\n" \
"void main(void)\n" \
"{\n" \
"	float light = dot(normalize(f_normal),normalize(vec3(0.5,1.0,0.1))) + 0.1;\n" \
"	gl_FragData[0] = vec4(u_color.rgb*light,u_color.a);\n" \
"	if(u_istextured0!=0)\n" \
"	{\n" \
"		gl_FragData[0]*=texture2D(u_texture0,f_texcoord0.st);\n" \
"	}\n" \
"}\n" \
""

#define _blit_v \
"#version 100\n" \
"\n" \
"uniform vec2 u_size;\n" \
"\n" \
"attribute vec3 a_position;\n" \
"\n" \
"varying vec2 f_pos;\n" \
"varying vec2 f_offset;\n" \
"\n" \
"void main(void)\n" \
"{\n" \
"	f_offset=vec2(1.0/u_size.x,1.0/u_size.y);\n" \
"	f_pos=a_position.xy+vec2(0.5);\n" \
"	gl_Position=vec4(a_position.xyz*2.0,1.0);\n" \
"}\n"

#define _blit_f \
"#version 100\n" \
"\n" \
"precision highp float;\n" \
"\n" \
"uniform sampler2D u_source0;\n" \
"uniform sampler2D u_source1;\n" \
"uniform sampler2D u_source2;\n" \
"uniform sampler2D u_source3;\n" \
"\n" \
"uniform vec4 u_data0;\n" \
"uniform vec4 u_data1;\n" \
"\n" \
"varying vec2 f_pos;\n" \
"varying vec2 f_offset;\n" \
"\n" \
"void main(void)\n" \
"{\n" \
"	vec4 color = vec4(texture2D(u_source0,f_pos).rgb,1.0);\n" \
"\n" \
"	gl_FragData[0] = color;\n" \
"}\n"


#endif