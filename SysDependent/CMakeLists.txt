# set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(MODULE_NAME aengine_native_window)
# we adds those files mannualy, 'coz it's not too much files here, and probably never won't,
# moreover those files are platform dependent

set(LOCAL_OTHER_LIBS "")
set(LOCAL_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/src/AEWindow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/AEWindowFactory.cpp)

if(TARGET_PLATFORM MATCHES "Android")
	add_library(android_glue OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/src/Android/native_app_glue/android_native_app_glue.c)
	set(LOCAL_SRCS ${LOCAL_SRCS}
		${CMAKE_CURRENT_SOURCE_DIR}/src/Android/AEAndroidWindow.cpp
		$<TARGET_OBJECTS:android_glue>)
	set(LOCAL_OTHER_LIBS EGL GLESv2 android log)
else()
	set(LOCAL_SRCS ${LOCAL_SRCS} 
		${CMAKE_CURRENT_SOURCE_DIR}/src/SDL/AESDLWindow.cpp)
	set(LOCAL_OTHER_LIBS SDL)
endif()

add_library(ae_debug ${CMAKE_CURRENT_SOURCE_DIR}/src/AEDebug.cpp)
add_library(${MODULE_NAME} ${LOCAL_SRCS})

target_link_libraries(${MODULE_NAME} ${LOCAL_OTHER_LIBS} ae_debug)

set(AE_LINK_COMPONENTS ${AE_LINK_COMPONENTS} ${MODULE_NAME} ae_debug CACHE INTERNAL "")

