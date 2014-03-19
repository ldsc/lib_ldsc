# Create bin directory (if ther isn`t)
file(MAKE_DIRECTORY bin)

# Create executables
add_executable(bin/abertura_dilatacao
   AberturaDilatacao/aberturaDilatacao.cpp
   AberturaDilatacao/CUtil.cpp)
target_link_libraries(bin/abertura_dilatacao ldsc)

add_executable(bin/abertura_dilatacao3D
   AberturaDilatacao/aberturaDilatacao3D.cpp
   AberturaDilatacao/CUtil.cpp)
target_link_libraries(bin/abertura_dilatacao3D ldsc)

add_executable(bin/criaSintetica3D
   ImagemSintetica3D/criaImagemSintetica.cpp)
target_link_libraries(bin/criaSintetica3D ldsc)

add_executable(bin/criaSintetica3D4p4g
   ImagemSintetica3D/criaImagemSintetica4p4g.cpp)
target_link_libraries(bin/criaSintetica3D4p4g ldsc)

add_executable(bin/idf3D
   Filtro/idf3D.cpp)
target_link_libraries(bin/idf3D ldsc)

add_executable(bin/criarBola
   Geometria/geometria.cpp)
target_link_libraries(bin/criarBola ldsc)

add_executable(bin/Permeabilidade_GrafoConexaoSerial
   Grafo/CGrafoTeste.cpp 
   Grafo/grafo.cpp)
target_link_libraries(bin/Permeabilidade_GrafoConexaoSerial ldsc)
