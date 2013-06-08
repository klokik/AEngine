# set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/include})

find_package(COLLADA_DOM 2.4.0 REQUIRED)
find_package(Boost COMPONENTS system filesystem)

# WHY IT DOESN'T WORK???
#include_directories(/usr/include/collada-dom2.4/)
#include_directories(/usr/include/collada-dom2.4/1.5/)

# add all content of src folder as libraries
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src DIR_LIST)
foreach(file_name ${DIR_LIST})
	get_filename_component(lib_name ${file_name} NAME_WE)
	add_library(${lib_name} ${file_name})
	set(LINK_LIBRARIES ${LINK_LIBRARIES} ${lib_name} CACHE INTERNAL "link libraries")
	set(SOURCE_FILES ${SOURCE_FILES} ${file_name} CACHE INTERNAL "source files")
endforeach()

#message("${COLLADA_DOM_INCLUDE_DIRS}")

set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} ${COLLADA_DOM_LIBRARIES} ${Boost_LIBRARIES} CACHE INTERNAL "additional libraries")
