#link_directories(${LIBLDSC_SOURCE_DIR}/src/)
add_executable(criaSintetica3D criaImagemSintetica.cpp)
target_link_libraries(criaSintetica3D ldsc)
