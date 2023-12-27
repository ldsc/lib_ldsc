########################### Gtests #############################
# Include and Link directories for Gtest.
include_directories(${LIBLDSC_SOURCE_DIR}/../gtest/include/)
link_directories(${LIBLDSC_SOURCE_DIR}/../gtest/)

#create bin directory
file(MAKE_DIRECTORY bin)

# Create executable
#add_executable(bin/runGtest
add_executable(runGtest
   Caracterizacao/RedePercolacao/gtRedeDePercolacao.cpp
   Caracterizacao/Simulacao/Permeabilidade/gtPermeabilidade.cpp
   Filtro/FEspacial/FEMorfologiaMatematica/gtFEMMIDFd3453D.cpp
   Filtro/FEspacial/FEMorfologiaMatematica/gtFEMMIRA3D.cpp
   Filtro/FEspacial/FEMorfologiaMatematica/gtFEMM3D.cpp
   Filtro/FEspacial/FEMorfologiaMatematica/gtFEMMIDFd34.cpp
   Filtro/FEspacial/FEMorfologiaMatematica/gtFEMMIRA.cpp
   Filtro/FEspacial/FERotulagem/gtRotulador3D.cpp
   Filtro/FEspacial/gtFEspacial.cpp
   Matriz/gtMatriz2D.cpp
   Segmentacao/PorosGargantas/gtAberturaDilatacao3D.cpp
   Segmentacao/PorosGargantas/gtSegPorosGargantas3D.cpp
   gtest_main.cpp
   gtest_configure.h
   fixtures.h
)

# Link executable
#target_link_libraries(bin/runGtest gtest ldsc)
target_link_libraries(runGtest gtest ldsc)
