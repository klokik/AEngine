# set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/include)

# we adds those files mannualy, 'coz it's not too much files here, and probably never won't,
# moreover those files are platform dependent

add_library(AEWindow ${CMAKE_CURRENT_SOURCE_DIR}/src/AEWindow.cpp)
add_library(AEWindowFactory ${CMAKE_CURRENT_SOURCE_DIR}/src/AEWindowFactory.cpp)
add_library(AESDLWindow ${CMAKE_CURRENT_SOURCE_DIR}/src/SDL/AESDLWindow.cpp)

set(LINK_LIBRARIES ${LINK_LIBRARIES} AEWindow AESDLWindow CACHE INTERNAL "link libraries")
set(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/AEWindow.cpp CACHE INTERNAL "source files")
set(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/AEWindowFactory.cpp CACHE INTERNAL "source files")
set(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/SDL/AESDLWindow.cpp CACHE INTERNAL "source files")

# add following as options
set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} SDL CACHE INTERNAL "additional libraries")
