set(MODULE_NAME aengine_glsl_render)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src LOCAL_SRCS)

add_library(${MODULE_NAME} ${LOCAL_SRCS} $<TARGET_OBJECTS:aengine_base>)
if(NOT BUILD_GLES_RENDER)
	target_link_libraries(${MODULE_NAME} GL)
else()
	target_link_libraries(${MODULE_NAME} ${GLES_LIB})
endif()

set(AE_LINK_COMPONENTS ${AE_LINK_COMPONENTS} ${MODULE_NAME} CACHE INTERNAL "")
