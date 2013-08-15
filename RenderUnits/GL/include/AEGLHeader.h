#ifndef AEGLHEADER_H_
#define AEGLHEADER_H_

#include "render_build_config.h"

#if defined(BUILD_GLES_RENDER)

#include <GLES2/gl2.h>
#define AE_NEW_GL_CONTEXT

#elif defined(BUILD_GL_RENDER)||defined(BUILD_GLSL_RENDER)

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GL/gl.h>

#endif


#endif /* AEGLHEADER_H_ */