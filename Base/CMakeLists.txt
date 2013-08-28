# set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(LOCAL_LIBS "")

# add all content of src folder as libraries
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src DIR_LIST)
# it will add content of Util directory
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/Utils DIR_LIST)
#foreach(file_name ${DIR_LIST})
#	get_filename_component(lib_name ${file_name} NAME_WE)
	#add_library(${lib_name} ${file_name})
	#set(LOCAL_LIBS ${LOCAL_LIBS} ${lib_name})
#endforeach()

set(LOCAL_SRCS ${DIR_LIST})
add_library(aengine_base OBJECT ${LOCAL_SRCS})
add_library(aengine_base_static $<TARGET_OBJECTS:aengine_base> $<TARGET_OBJECTS:aengine_render_factory>)

set(AE_LINK_COMPONENTS ${AE_LINK_COMPONENTS} aengine_base_static CACHE INTERNAL "")

#add following as option
set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} png CACHE INTERNAL "")
