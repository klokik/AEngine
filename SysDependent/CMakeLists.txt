# set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/include)

# we adds those files mannualy, 'coz it's not too much files here, and probably never won't,
# moreover those files are platform dependent

add_library(AEWindow ${CMAKE_CURRENT_SOURCE_DIR}/src/AEWindow.cpp)
add_library(AEWindowFactory ${CMAKE_CURRENT_SOURCE_DIR}/src/AEWindowFactory.cpp)

set(LOCAL_LIBS AEWindow AEWindowFactory)

if(TARGET_PLATFORM MATCHES "Android")
	add_library(AEAndroidWindow
		${CMAKE_CURRENT_SOURCE_DIR}/src/Android/AEAndroidWindow.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/Android/native_app_glue/android_native_app_glue.c )
	set(LOCAL_LIBS ${LOCAL_LIBS} AEAndroidWindow)
	set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} EGL GLESv2 android log CACHE INTERNAL "additional libraries")
	set(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/Android/AEAndroidWindow.cpp CACHE INTERNAL "source files")
	set(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/Android/native_app_glue/android_native_app_glue.c CACHE INTERNAL "source files")
else()
	add_library(AESDLWindow ${CMAKE_CURRENT_SOURCE_DIR}/src/SDL/AESDLWindow.cpp)
	set(LOCAL_LIBS ${LOCAL_LIBS} AESDLWindow)
	set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} SDL CACHE INTERNAL "additional libraries")
	set(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/SDL/AESDLWindow.cpp CACHE INTERNAL "source files")
endif()

set(LINK_LIBRARIES ${LINK_LIBRARIES} ${LOCAL_LIBS} INTERNAL "link libraries")
set(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/AEWindow.cpp CACHE INTERNAL "source files")
set(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/AEWindowFactory.cpp CACHE INTERNAL "source files")

# add following as options
