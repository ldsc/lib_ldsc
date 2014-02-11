add_subdirectory(SMDiagonal)

include_directories (${LIBLDSC_SOURCE_DIR}/src/SMatriz/SMDiagonal)

link_directories (${LIBLDSC_BINARY_DIR}/src/SMatriz/SMDiagonal)

add_library(SMatriz CSMatriz.cpp CSMParametroSolver.cpp)

target_link_libraries(SMatriz SMDiagonal)