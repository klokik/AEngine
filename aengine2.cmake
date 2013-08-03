set("${PROJECT}_VERSION_MAJOR" 2)
set("${PROJECT}_VERSION_MAJOR" 0)

add_definitions(-std=c++11)

set(CMAKE_BUILD_TYPE Debug)

add_definitions(-Wall)
# set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-rdynamic")

#set configuration header file for source code
add_definitions(-DPLATFORM_xNIX)

option(BUILD_EXECUTABLE "Build either executable binary or shared lib." ON)
set(BUILD_EXECUTABLE ${BUILD_EXECUTABLE} CACHE INTERNAL "" FORCE)

option(TARGET_PLATFORM "Platform where application would be runned." "xNIX")
set(TARGET_PLATFORM ${TARGET_PLATFORM} CACHE INTERNAL "" FORCE)

message("Target platform: ${TARGET_PLATFORM}")
if(${TARGET_PLATFORM} MATCHES "Android")
	set(BUILD_EXECUTABLE OFF CACHE INTERNAL "" FORCE)
endif()

if(NOT BUILD_EXECUTABLE)
	if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
#		set_target_properties(bar PROPERTIES COMPILE_FLAGS "-fPIC")
		add_definitions(-fPIC)
	endif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
endif()
