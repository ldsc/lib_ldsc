#link_directories(${LIBLDSC_SOURCE_DIR}/src/)
add_executable(abertura_dilatacao aberturaDilatacao.cpp CUtil.cpp)
target_link_libraries(abertura_dilatacao ldsc)

#add_executable(abertura_dilatacao_old aberturaDilatacaoOld.cpp CUtil.cpp)
#target_link_libraries(abertura_dilatacao_old ldsc)

add_executable(abertura_dilatacao3D aberturaDilatacao3D.cpp CUtil.cpp)
target_link_libraries(abertura_dilatacao3D ldsc)

