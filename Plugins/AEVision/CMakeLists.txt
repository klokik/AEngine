# set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/include})

# add all content of src folder as libraries
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src DIR_LIST)
foreach(file_name ${DIR_LIST})
	get_filename_component(lib_name ${file_name} NAME_WE)
	add_library(${lib_name} ${file_name})
	set(LINK_LIBRARIES ${LINK_LIBRARIES} ${lib_name} CACHE INTERNAL "link libraries")
	set(SOURCE_FILES ${SOURCE_FILES} ${file_name} CACHE INTERNAL "source files")
endforeach()

find_package(OpenCV REQUIRED)
set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} ${OpenCV_LIBS} CACHE INTERNAL "additional libraries")
