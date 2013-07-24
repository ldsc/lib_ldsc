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

add_executable(bin/idf3D
   Filtro/MM/idf3D.cpp)
target_link_libraries(bin/idf3D ldsc)

#add_executable(bin/test main.cpp CTeste.cpp)
