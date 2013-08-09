/*
 * AEDefines.h
 *
 *  Created on: Aug 19, 2011
 *      Author: klokik
 */

#ifndef AEDEFINES_H_
#define AEDEFINES_H_

typedef void  (*AE_CALLBACK)(void *);
typedef void  (*AE_EVENT)(int *);

#define AE_FALSE	0x00
#define AE_TRUE		0x01

#define AE_OK		0x01
#define AE_ERR		0x00

#define AE_TYPE	unsigned int

#define AE_WINDOW_TYPE		AE_TYPE
#define AE_SDL_WINDOW		0x20
#define AE_SDLGL_WINDOW		0x21
#define AE_SDLEGL_WINDOW	0x22

#define AE_INIT_SCENE		0x0001
#define AE_INIT_WINDOW		0x0002
#define AE_INIT_CAMERA		0x0004
#define AE_INIT_RENDER_GL	0x0008
#define AE_INIT_RENDER_D3D	0x0010
#define AE_INIT_RENDER_OTHER	0x0020
#define AE_INIT_AUDIO		0x0040

#define AE_RENDER_GL		0x0008
#define AE_RENDER_GLSL		0x0009
#define AE_RENDER_D3D		0x0010
#define AE_RENDER_GLES		0x0011

//TODO: move this definitions to separete header files where this objects are defined
// as it's inconvinient to rebuild the whole project after changing one definition
#define AE_OBJ_TYPE			AE_TYPE
#define AE_OBJ_GENERIC		0x0000
#define AE_OBJ_EMPTY		0x0001
#define AE_OBJ_MESH			0x0002
#define AE_OBJ_SURFACE		0x0003
#define AE_OBJ_LIGHT		0x0004
#define AE_OBJ_CAMERA		0x0005
#define AE_OBJ_SPRITE		0x0006
#define AE_OBJ_TEXT			0x0008
#define AE_OBJ_JOINT		0x0009

#define AE_LIGHT_TYPE		 int
#define AE_AMBIENT_LIGHT	 0x01
#define AE_DIRECTIONAL_LIGHT 0x02
#define AE_POINT_LIGHT		 0x03
#define AE_SPOT_LIGHT		 0x04

#define AE_EVENT_CLOSE		0x80
#define AE_EVENT_KEYDOWN	0x81
#define AE_EVENT_KEYUP		0x82
#define AE_EVENT_MOUSEDOWN	0x83
#define AE_EVENT_MOUSEMOVE	0x84
#define AE_EVENT_MOUSEUP	0x85
#define AE_EVENT_RESIZE		0x86
#define AE_EVENT_START		0x87
#define AE_REFRESH			0x88

#define AE_KEY				short int

#define AE_STATIC	0x80
#define AE_DYNAMIC	0x81

#define AE_ORTHOGRAPHIC	0x82
#define AE_PERSPECTIVE	0x83

#define AE_VERTEX_BIT		0x01
#define AE_NORMAL_BIT		0x02
#define AE_COLOR_BIT		0x04
#define AE_TEXTURE_BIT		0x08

#define AE_TC_ARR_EMPTY		0x01
#define AE_TC_ARR_MESH		0x02
#define AE_TC_ARR_SPRITE_PERSP	0x03
#define AE_TC_ARR_SPRITE_ORTHO	0x04
#define AE_TC_ARR_LIGHT		0x05
#define AE_TC_ARR_CAMERA	0x06

#define AE_LEFT		0x01
#define AE_RIGHT	0x02
#define AE_CENTER	0x04


//Macro utils
// #define AECOORD(vec) vec.X,vec.Y,vec.Z

#endif /* AEDEFINES_H_ */
