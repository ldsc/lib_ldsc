add_subdirectory(FEDeteccaoContorno)
add_subdirectory(FEMorfologiaMatematica)
add_subdirectory(FEEsqueleto)
add_subdirectory(FEInversao)

include_directories (${LIBLDSC_SOURCE_DIR}/src/Filtro/FEDeteccaoContorno)
include_directories (${LIBLDSC_SOURCE_DIR}/src/Filtro/FEMorfologiaMatematica)
include_directories (${LIBLDSC_SOURCE_DIR}/src/Filtro/FEEsqueleto)
include_directories (${LIBLDSC_SOURCE_DIR}/src/Filtro/FEInversao)

link_directories (${LIBLDSC_BINARY_DIR}/src/Filtro/FEDeteccaoContorno)
link_directories (${LIBLDSC_BINARY_DIR}/src/Filtro/FEMorfologiaMatematica)
link_directories (${LIBLDSC_BINARY_DIR}/src/Filtro/FEEsqueleto)
link_directories (${LIBLDSC_BINARY_DIR}/src/Filtro/FEInversao)

add_library(FEspacial CFEspacial.cpp CFEBartlet.cpp CFEDeteccaoContornoMasc.cpp CFEGaussiano.cpp CFEPassaAlta.cpp  CFEBorda.cpp CFEEliminaBorda.cpp CFELaplaciano.cpp CFEPassaBaixa.cpp CFEBSubstitueValor.cpp CFEEmboss.cpp CFEPassaAlta2.cpp CFERotacaoVerticalEDiferenca.cpp)

target_link_libraries(FEspacial FEDeteccaoContorno FEMorfologiaMatematica FEEsqueleto FEInversao)
