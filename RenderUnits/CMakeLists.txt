option(BUILD_GL_RENDER "Desktop OpenGL 1.4 render unit" ON)
option(BUILD_GLSL_RENDER "Desktop OpenGL 2.1+ render unit" ON)
option(BUILD_GLES_RENDER "Mobile OpenGLES 2.0+ render unit" OFF)

message("Build GL: ${BUILD_GL_RENDER}")
message("Build GLSL: ${BUILD_GLSL_RENDER}")
message("Build GLES: ${BUILD_GLES_RENDER}")

if(BUILD_GL_RENDER OR BUILD_GLSL_RENDER)
	add_definitions(-DBUILD_GL_RENDER)
	add_subdirectory(GL)
endif()

if(BUILD_GLSL_RENDER)
	add_definitions(-DBUILD_GLSL_RENDER)
	add_subdirectory(GLSL)
endif()

if(BUILD_GLES_RENDER)
	add_definitions(-DBUILD_GLES_RENDER)
	add_subdirectory(GLES)
endif()
