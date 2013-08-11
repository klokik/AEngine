set(MODULE_NAME aengine_gles_render)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src LOCAL_SRCS)

add_library(${MODULE_NAME} ${LOCAL_SRCS} $<TARGET_OBJECTS:aengine_base>)
target_link_libraries(${MODULE_NAME} EGL GLESv2)

set(AE_LINK_COMPONENTS ${AE_LINK_COMPONENTS} ${MODULE_NAME} CACHE INTERNAL "")