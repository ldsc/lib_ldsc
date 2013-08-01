add_executable(bin/abertura_dilatacao
   Old/AberturaDilatacao/aberturaDilatacao.cpp
   Old/AberturaDilatacao/CUtil.cpp)
target_link_libraries(bin/abertura_dilatacao ldsc)

add_executable(bin/abertura_dilatacao3D
   Old/AberturaDilatacao/aberturaDilatacao3D.cpp
   Old/AberturaDilatacao/CUtil.cpp)
target_link_libraries(bin/abertura_dilatacao3D ldsc)

add_executable(bin/criaSintetica3D
   Old/ImagemSintetica3D/criaImagemSintetica.cpp)
target_link_libraries(bin/criaSintetica3D ldsc)

add_executable(bin/idf3D
   Old/Filtro/idf3D.cpp)
target_link_libraries(bin/idf3D ldsc)

########################### Gtests #############################

include_directories(${LIBLDSC_SOURCE_DIR}/../gtest/include/)
link_directories(${LIBLDSC_SOURCE_DIR}/../gtest/)

add_executable(bin/runGtest
   Filtro/FEspacial3D/FEMorfologiaMatematica3D/gtFEMMIDFd3453D.cpp
   Filtro/FEspacial/FEMorfologiaMatematica/gtFEMMIDFd34.cpp
   Segmentacao/3D/PorosGargantas/gtAberturaDilatacao3D.cpp
   Filtro/FEspacial/TCFEPassaBaixaTests.cpp
   gtest_main.cpp
   gtest_configure.h
)

target_link_libraries(bin/runGtest gtest ldsc)
