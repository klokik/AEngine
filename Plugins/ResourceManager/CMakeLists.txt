set(MODULE_NAME aengine_resource_manager)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src LOCAL_SRCS)

add_library(${MODULE_NAME} ${LOCAL_SRCS} $<TARGET_OBJECTS:aengine_base>)
# target_link_libraries(${MODULE_NAME})

set(AE_LINK_COMPONENTS ${AE_LINK_COMPONENTS} ${MODULE_NAME} CACHE INTERNAL "")
