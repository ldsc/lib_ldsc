add_subdirectory(FAmplitude)
add_subdirectory(FEspacial)
add_subdirectory(FEspacial3D)
add_subdirectory(FReconstrucao)

include_directories (${LIBLDSC_SOURCE_DIR}/src/Filtro/FAmplitude)
include_directories (${LIBLDSC_SOURCE_DIR}/src/Filtro/FEspacial)
include_directories (${LIBLDSC_SOURCE_DIR}/src/Filtro/FEspacial3D)
include_directories (${LIBLDSC_SOURCE_DIR}/src/Filtro/FReconstrucao)

link_directories (${LIBLDSC_BINARY_DIR}/src/Filtro/FAmplitude)
link_directories (${LIBLDSC_BINARY_DIR}/src/Filtro/FEspacial)
link_directories (${LIBLDSC_BINARY_DIR}/src/Filtro/FEspacial3D)
link_directories (${LIBLDSC_BINARY_DIR}/src/Filtro/FReconstrucao)

add_library(Filtro CFiltro.cpp CFiltro3D.cpp)

target_link_libraries(Filtro FAmplitude FEspacial FEspacial3D FReconstrucao)