add_subdirectory(FEConectividade3D)
add_subdirectory(FEEsqueleto3D)
add_subdirectory(FEInversao3D)
add_subdirectory(FEMorfologiaMatematica3D)

include_directories (${LIBLDSC_SOURCE_DIR}/src/Filtro/FEConectividade3D)
include_directories (${LIBLDSC_SOURCE_DIR}/src/Filtro/FEEsqueleto3D)
include_directories (${LIBLDSC_SOURCE_DIR}/src/Filtro/FEInversao3D)
include_directories (${LIBLDSC_SOURCE_DIR}/src/Filtro/FEMorfologiaMatematica3D)

link_directories (${LIBLDSC_BINARY_DIR}/src/Filtro/FEConectividade3D)
link_directories (${LIBLDSC_BINARY_DIR}/src/Filtro/FEEsqueleto3D)
link_directories (${LIBLDSC_BINARY_DIR}/src/Filtro/FEInversao3D)
link_directories (${LIBLDSC_BINARY_DIR}/src/Filtro/FEMorfologiaMatematica3D)

add_library(FEspacial3D CFEspacial3D.cpp CFEBorda3D.cpp CFEBSubstitueValor3D.cpp)

target_link_libraries(FEspacial3D FEConectividade3D FEEsqueleto3D FEInversao3D FEMorfologiaMatematica3D)