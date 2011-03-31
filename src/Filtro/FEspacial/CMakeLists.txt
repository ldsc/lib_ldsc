add_subdirectory(FEDeteccaoContorno)
add_subdirectory(FEMorfologiaMatematica)
add_subdirectory(FEEsqueleto)
add_subdirectory(FEInversao)

add_library(FEspacial CFEspacial.cpp CFEBartlet.cpp CFEDeteccaoContornoMasc.cpp CFEGaussiano.cpp CFEPassaAlta.cpp  CFEBorda.cpp CFEEliminaBorda.cpp CFELaplaciano.cpp CFEPassaBaixa.cpp CFEBSubstitueValor.cpp CFEEmboss.cpp CFEPassaAlta2.cpp CFERotacaoVerticalEDiferenca.cpp)