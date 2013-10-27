set(MODULE_NAME aengine_collada_loader)

find_package(COLLADA_DOM 2.4.0 REQUIRED)
find_package(Boost COMPONENTS system filesystem)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src LOCAL_SRCS)

add_library(${MODULE_NAME} ${LOCAL_SRCS} $<TARGET_OBJECTS:aengine_base>)
target_link_libraries(${MODULE_NAME} ${COLLADA_DOM_LIBRARIES} ${Boost_LIBRARIES})

set(AE_LINK_COMPONENTS ${AE_LINK_COMPONENTS} ${MODULE_NAME} CACHE INTERNAL "")
