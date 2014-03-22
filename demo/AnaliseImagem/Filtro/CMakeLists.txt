# Create bin directory (if ther isn`t)
#file(MAKE_DIRECTORY bin)

add_subdirectory(FAmplitude)
add_subdirectory(FEspacial)
add_subdirectory(FFrequencial)
add_subdirectory(FReconstrucao)
add_subdirectory(Geometria)
add_subdirectory(Mascara)

# add_executable(bin/Permeabilidade_GrafoConexaoSerial
#    GrafoConexaoSerial/CGrafoConexaoSerial_Teste.cpp
#    GrafoConexaoSerial/grafo_main.cpp)
# target_link_libraries(bin/Permeabilidade_GrafoConexaoSerial ldsc)
