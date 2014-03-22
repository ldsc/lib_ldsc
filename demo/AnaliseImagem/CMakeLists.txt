# Create bin directory (if ther isn`t)
#file(MAKE_DIRECTORY bin)

add_subdirectory(Amostra)
add_subdirectory(AnaliseImagem)
add_subdirectory(Base)
add_subdirectory(Estatistica)
add_subdirectory(EstruturaDados)
add_subdirectory(Grafico)
add_subdirectory(InteligenciaArtificial)
add_subdirectory(MetNum)
add_subdirectory(Tempo)

# add_executable(bin/Permeabilidade_GrafoConexaoSerial
#    GrafoConexaoSerial/CGrafoConexaoSerial_Teste.cpp
#    GrafoConexaoSerial/grafo_main.cpp)
# target_link_libraries(bin/Permeabilidade_GrafoConexaoSerial ldsc)
