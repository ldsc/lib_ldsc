add_subdirectory(Interpolacao2D)
add_subdirectory(IntMetNum)

add_library(Interpolacao CIntLagrange.cpp CINewtonDiff.cpp CINewtonDivDiff.cpp CIntBarycentric.cpp CIntCubSpline.cpp  CIntEDBarycentric.cpp CInterpolacao.cpp)