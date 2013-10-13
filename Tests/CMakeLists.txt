if(BUILD_EXECUTABLE)
	add_executable(matrix_test matrix.cpp ${DIR_LIST})
	target_link_libraries(matrix_test ${LOCAL_LIBS} ${AE_BINARY_NAME})
endif()