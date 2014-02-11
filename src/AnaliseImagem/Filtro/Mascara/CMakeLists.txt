add_subdirectory(MCDiscreta)

include_directories (${LIBLDSC_SOURCE_DIR}/src/Mascara/MCDiscreta)

link_directories (${LIBLDSC_BINARY_DIR}/src/Mascara/MCDiscreta)

add_library(Mascara CMascara.cpp CMPassaBaixa.cpp CMPAssaAlta.cpp CMBartlet.cpp CMEmboss.cpp CMLaplaciano.cpp CMDeteccaoContorno.cpp CMGaussiano.cpp CMPassaAlta2.cpp CMRotacaoVerticaleDiferenca.cpp CMascara3D.cpp CMPassaBaixa3d.cpp)

target_link_libraries(Mascara MCDiscreta)