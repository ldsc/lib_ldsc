# Create bin directory (if ther isn`t)
#file(MAKE_DIRECTORY bin)

add_subdirectory(Correlacao)
add_subdirectory(Distribuicao)
add_subdirectory(Fractal)
add_subdirectory(GrafoConexaoSerial)
add_subdirectory(RedePercolacao)

# add_executable(bin/Permeabilidade_GrafoConexaoSerial
#    GrafoConexaoSerial/CGrafoConexaoSerial_Teste.cpp
#    GrafoConexaoSerial/grafo_main.cpp)
# target_link_libraries(bin/Permeabilidade_GrafoConexaoSerial ldsc)
