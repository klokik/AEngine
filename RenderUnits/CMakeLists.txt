option(BUILD_GL_RENDER "Desktop OpenGL 1.4 render unit" ON)
option(BUILD_GLSL_RENDER "Desktop OpenGL 2.1+ render unit" ON)
option(BUILD_GLES_RENDER "Mobile OpenGLES 2.0+ render unit" ON)

message("Build GL: ${BUILD_GL_RENDER}")
message("Build GLSL: ${BUILD_GLSL_RENDER}")
message("Build GLES: ${BUILD_GLES_RENDER}")

configure_file(${AE_CONFIG_DIR}/render_build_config.h.in
	${AE_CONFIG_DIR}/render_build_config.h)

set(render_units "")

if(TARGET_PLATFORM STREQUAL "xNIX")
	set(GLES_LIB GLESv2)
elseif(TARGET_PLATFORM STREQUAL "Android")
#	set(GLES_LIB GLESv3)
	set(GLES_LIB GLESv2)
else()
	message(SEND_ERROR "Unknown platform, trying to use GLESv2")
	set(GLES_LIB GLESv2)
endif()

if(BUILD_GL_RENDER OR BUILD_GLSL_RENDER)
	set(render_units ${render_units} "aengine_gl_render")
	add_subdirectory(GL)
endif()

if(BUILD_GLSL_RENDER)
	set(render_units ${render_units} "aengine_glsl_render")
	add_subdirectory(GLSL)
endif()

if(BUILD_GLES_RENDER)
	set(render_units ${render_units} "aengine_gles_render")
	add_subdirectory(GLES)
endif()

add_library(aengine_render_factory OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/src/AERenderFactory.cpp)
add_library(aengine_render_factory_static $<TARGET_OBJECTS:aengine_render_factory>)
target_link_libraries(aengine_render_factory_static ${render_units} ae_debug)

set(AE_LINK_COMPONENTS ${AE_LINK_COMPONENTS} aengine_render_factory_static CACHE INTERNAL "")
