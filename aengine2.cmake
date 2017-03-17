set("${PROJECT}_VERSION_MAJOR" 2)
set("${PROJECT}_VERSION_MAJOR" 0)

add_definitions(-std=c++11)

set(CMAKE_BUILD_TYPE Debug)

add_definitions(-Wall)
# set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-rdynamic")


option(BUILD_EXECUTABLE "Build either executable binary or shared lib." ON)
set(BUILD_EXECUTABLE ${BUILD_EXECUTABLE} CACHE INTERNAL "")

set(TARGET_PLATFORM "xNIX" CACHE STRING "Platform where application would be runned.")

set(AE_BINARY_NAME aengine)

set(AE_CONFIG_DIR ${CMAKE_SOURCE_DIR}/build_configs)

#set build output folders
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build )

message("Target platform: ${TARGET_PLATFORM}")
if(TARGET_PLATFORM STREQUAL "Android")
	set(BUILD_EXECUTABLE OFF CACHE INTERNAL "" FORCE)
	set(AE_BINARY_NAME aengine-android)
	add_definitions(-DPLATFORM_ANDROID)
elseif(TARGET_PLATFORM STREQUAL "xNIX" OR TARGET_PLATFORM STREQUAL "RPI")
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs )

	add_definitions(-DPLATFORM_xNIX)
	if(TARGET_PLATFORM STREQUAL "RPI")
		add_definitions(-DPLATFORM_RPI)
	endif()
endif()

if(NOT BUILD_EXECUTABLE)
	if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
		# set_target_properties(bar PROPERTIES COMPILE_FLAGS "-fPIC")
		add_definitions(-fPIC)
	endif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")

	set(AE_LINK SHARED)
else()
	set(AE_LINK STATIC)
endif()

#set configuration header file for source code

