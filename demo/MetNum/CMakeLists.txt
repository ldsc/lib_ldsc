# Create bin directory (if ther isn`t)
#file(MAKE_DIRECTORY bin)

add_subdirectory(Contorno)
add_subdirectory(Funcao)
add_subdirectory(Integral)
add_subdirectory(Interpolacao)
add_subdirectory(Matriz)
add_subdirectory(Solver)

# add_executable(bin/Permeabilidade_GrafoConexaoSerial
#    GrafoConexaoSerial/CGrafoConexaoSerial_Teste.cpp
#    GrafoConexaoSerial/grafo_main.cpp)
# target_link_libraries(bin/Permeabilidade_GrafoConexaoSerial ldsc)
