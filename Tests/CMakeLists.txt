if(BUILD_EXECUTABLE)
	add_executable(matrix_test matrix.cpp ${DIR_LIST})
	target_link_libraries(matrix_test ${LOCAL_LIBS} ${AE_BINARY_NAME})

	add_executable(city city.cpp ${DIR_LIST})
	target_link_libraries(city ${LOCAL_LIBS} ${AE_BINARY_NAME})

	add_executable(random_stuff main.cpp ${DIR_LIST})
	target_link_libraries(random_stuff ${LOCAL_LIBS} ${AE_BINARY_NAME})

	add_executable(particles particles.cpp)
	target_link_libraries(particles ${LOCAL_LIBS} ${AE_BINARY_NAME})
endif()

