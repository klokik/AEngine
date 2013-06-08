# set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/include})

# add all content of src folder as libraries
set(LOCAL_SOURCES "")
set(LOCAL_LIBS "")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/. DIR_LIST)
foreach(file_name ${DIR_LIST})
	get_filename_component(lib_name ${file_name} NAME_WE)
	add_library(${lib_name} ${file_name})
	set(LOCAL_LIBS ${LOCAL_LIBS} ${lib_name})
	set(LOCAL_SOURCES ${LOCAL_SOURCES} ${file_name})
endforeach()

# ${PROJECT_SOURCE_DIR}/Tests/ZombieWorld/main.cpp
add_executable(zombie ${SOURCE_FILES} ${LOCAL_SOURCES})
target_link_libraries(zombie ${LINK_LIBRARIES} ${LOCAL_LIBS} ${ADDITIONAL_LIBRARIES})
