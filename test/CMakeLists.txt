########################### Gtests #############################
# Include and Link directories for Gtest.
include_directories(${LIBLDSC_SOURCE_DIR}/../gtest/include/)
link_directories(${LIBLDSC_SOURCE_DIR}/../gtest/)

#create bin directory
file(MAKE_DIRECTORY bin)

# Create executable
add_executable(bin/runGtest
   Filtro/FEspacial3D/FEMorfologiaMatematica3D/gtFEMMIDFd3453D.cpp
   Filtro/FEspacial/FEMorfologiaMatematica/gtFEMMIDFd34.cpp
   Segmentacao/3D/PorosGargantas/gtAberturaDilatacao3D.cpp
   Filtro/FEspacial/FEspacialTests.cpp
   gtest_main.cpp
   gtest_configure.h
)

# Link executable
target_link_libraries(bin/runGtest gtest ldsc)
