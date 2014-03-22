# Create bin directory (if ther isn`t)
#file(MAKE_DIRECTORY bin)

add_subdirectory(Caracterizacao)
add_subdirectory(Filtro)
add_subdirectory(Geometria)
add_subdirectory(Reconstrucao)
add_subdirectory(Segmentacao)
add_subdirectory(Simulacao)

# add_executable(bin/Permeabilidade_GrafoConexaoSerial
#    GrafoConexaoSerial/CGrafoConexaoSerial_Teste.cpp
#    GrafoConexaoSerial/grafo_main.cpp)
# target_link_libraries(bin/Permeabilidade_GrafoConexaoSerial ldsc)
