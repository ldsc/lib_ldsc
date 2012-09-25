#link_directories(${LIBLDSC_SOURCE_DIR}/src/)
add_executable(matriz3D CMatriz3D.test.cpp)
target_link_libraries(matriz3D ldsc)
