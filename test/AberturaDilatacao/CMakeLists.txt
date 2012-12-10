#link_directories(${LIBLDSC_SOURCE_DIR}/src/)
add_executable(abertura_dilatacao CAberturaDilatacao.teste.cpp CUtil.cpp)
target_link_libraries(abertura_dilatacao ldsc)

add_executable(abertura_dilatacao_new CAberturaDilatacaoNew.teste.cpp CUtil.cpp)
target_link_libraries(abertura_dilatacao_new ldsc)
