cmake_minimum_required (VERSION 2.6)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

message("Starting cmake")

if (NOT UNIX)
	message(FATAL_ERROR "Not Unix")
endif()

project(AEngine2)

# import version and other properties from external file
include(aengine2.cmake)

#set build output folders
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build )

#we use this variable to add different include pathes thoughout project
set(INCLUDES "Tests" CACHE INTERNAL "include directories" FORCE)
# same with libraries that would be linked with executable/to shared library
set(LINK_LIBRARIES "" CACHE INTERNAL "link libraries" FORCE)
# and same with source files
set(SOURCE_FILES "" CACHE INTERNAL "source files" FORCE)

#set additional libraries to be linked with
set(ADDITIONAL_LIBRARIES "" CACHE INTERNAL "additional libraries" FORCE)

include(includes.cmake)
include_directories(${INCLUDES})

add_subdirectory(Base)
add_subdirectory(RenderUnits)
add_subdirectory(SysDependent)
add_subdirectory(Plugins)

# include_directories(${INCLUDES})


# list all libraries to be linked
# foreach(lib ${LINK_LIBRARIES})
# 	message("lib: " ${lib})
# endforeach() 

add_library(aengine ${SOURCE_FILES})
target_link_libraries(aengine ${LINK_LIBRARIES} ${ADDITIONAL_LIBRARIES})
# add_executable(ae_test ${PROJECT_SOURCE_DIR}/Tests/skel_test.cpp)
# target_link_libraries(ae_test aengine)

# Zombie World

# include_directories(Project/ZombieWorld)
# add_subdirectory(Project/ZombieWorld)

# Heaven

include_directories(Project/Heaven)
add_subdirectory(Project/Heaven)

