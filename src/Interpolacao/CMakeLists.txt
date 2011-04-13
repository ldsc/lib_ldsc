add_subdirectory(Interpolacao2D)
add_subdirectory(IntMetNum)

include_directories (${LIBLDSC_SOURCE_DIR}/src/Interpolacao/Interpolacao2D)
include_directories (${LIBLDSC_SOURCE_DIR}/src/Interpolacao/IntMetNum)

link_directories (${LIBLDSC_BINARY_DIR}/src/Interpolacao/Interpolacao2D)
link_directories (${LIBLDSC_BINARY_DIR}/src/Interpolacao/IntMetNum)

add_library(Interpolacao CIntLagrange.cpp CINewtonDiff.cpp CINewtonDivDiff.cpp CIntBarycentric.cpp CIntCubSpline.cpp  CIntEDBarycentric.cpp CInterpolacao.cpp)

target_link_libraries(Interpolacao Interpolacao2D IntMetNum)