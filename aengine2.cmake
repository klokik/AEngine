set("${PROJECT}_VERSION_MAJOR" 2)
set("${PROJECT}_VERSION_MAJOR" 0)

add_definitions(-std=c++11)

set(CMAKE_BUILD_TYPE Debug)

add_definitions(-Wall)
# set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-rdynamic")


option(BUILD_EXECUTABLE "Build either executable binary or shared lib." ON)
set(BUILD_EXECUTABLE ${BUILD_EXECUTABLE} CACHE INTERNAL "")

option(TARGET_PLATFORM "Platform where application would be runned." "xNIX")
set(TARGET_PLATFORM ${TARGET_PLATFORM} CACHE INTERNAL "" FORCE)

set(AE_BINARY_NAME aengine)

set(AE_CONFIG_DIR ${CMAKE_SOURCE_DIR}/build_configs)

message("Target platform: ${TARGET_PLATFORM}")
if(TARGET_PLATFORM STREQUAL "Android")
	set(BUILD_EXECUTABLE OFF CACHE INTERNAL FORCE)
	set(AE_BINARY_NAME aengine-android)
	add_definitions(-DPLATFORM_ANDROID)
elseif(TARGET_PLATFORM STREQUAL "xNIX")
	add_definitions(-DPLATFORM_xNIX)
endif()

if(NOT BUILD_EXECUTABLE)
	if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
#		set_target_properties(bar PROPERTIES COMPILE_FLAGS "-fPIC")
		add_definitions(-fPIC)
	endif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")

	set(AE_LINK SHARED)
else()
	set(AE_LINK STATIC)
endif()

#set configuration header file for source code

