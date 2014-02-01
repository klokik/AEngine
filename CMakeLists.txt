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

#we use this variable to add different include pathes thoughout project
set(INCLUDES "Tests" CACHE INTERNAL "" FORCE)
# same with libraries that would be linked with executable/to shared library
set(AE_LINK_COMPONENTS "" CACHE INTERNAL "" FORCE)

#set additional libraries to be linked with
set(ADDITIONAL_LIBRARIES "pthread" CACHE INTERNAL "" FORCE)

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

# ${AE_LINK}
add_library(${AE_BINARY_NAME} STATIC dummy.cpp)
target_link_libraries(${AE_BINARY_NAME} ${AE_LINK_COMPONENTS} ${ADDITIONAL_LIBRARIES})
# add_executable(ae_test ${PROJECT_SOURCE_DIR}/Tests/skel_test.cpp)
# target_link_libraries(ae_test aengine)

# Zombie World

# include_directories(Project/ZombieWorld)
# add_subdirectory(Project/ZombieWorld)

# Heaven

include_directories(Project/Heaven)
add_subdirectory(Project/Heaven)

# Tests

add_subdirectory(Tests)
