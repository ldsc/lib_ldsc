#add_subdirectory(Base) add_subdirectory(ConEqu) add_subdirectory(Contorno) add_subdirectory(Correlacao) add_subdirectory(Estatistica) add_subdirectory(FFT) add_subdirectory(Filtro) add_subdirectory(Funcao) add_subdirectory(Geometria) add_subdirectory(Grafico) add_subdirectory(Grafo) add_subdirectory(Integral) add_subdirectory(Interpolacao) add_subdirectory(Mascara) add_subdirectory(Material) add_subdirectory(Matriz) add_subdirectory(Permeabilidade) add_subdirectory(Reconstrucao) add_subdirectory(Rotulador) add_subdirectory(SFuncao) add_subdirectory(SMatriz) add_subdirectory(Tempo)

add_library (ldsc STATIC
#add_library (ldsc SHARED
  Amostra/Material/CMFluido.cpp
  AnaliseImagem/Caracterizacao/Correlacao/CCorrelacao.cpp
  AnaliseImagem/Caracterizacao/Correlacao/CCorrelacao3D.cpp
  AnaliseImagem/Caracterizacao/Correlacao/CCorrelacaoEspacial.cpp
  AnaliseImagem/Caracterizacao/Correlacao/CCorrelacaoFFT.cpp
  AnaliseImagem/Caracterizacao/Distribuicao/CBaseDistribuicao.cpp
  AnaliseImagem/Caracterizacao/Distribuicao/CDistribuicao.cpp
  AnaliseImagem/Caracterizacao/Distribuicao/CDistribuicao3D.cpp
  AnaliseImagem/Caracterizacao/Distribuicao/CDistribuicaoTamanhoPorosGargantas.cpp

  EstruturaDados/ObjetoGrafo/CObjetoGrafo.cpp
  EstruturaDados/ObjetoGrafo/CObjetoGrafo_1VetorConexoes.cpp
  EstruturaDados/ObjetoGrafo/CObjetoGrafo_2VetoresConexoes.cpp
  EstruturaDados/ObjetoGrafo/CObjetoGrafo_MatrizConexoes.cpp

  EstruturaDados/ObjetoRede/CObjetoRede.cpp
  EstruturaDados/ObjetoRede/CObjetoRede_Ligacao.cpp
  EstruturaDados/ObjetoRede/CObjetoRede_Ligacao_EST.cpp
  EstruturaDados/ObjetoRede/CObjetoRede_Ligacao_WEST.cpp
  EstruturaDados/ObjetoRede/CObjetoRede_Sitio.cpp
  EstruturaDados/ObjetoRede/CObjetoRede_Sitio_EST.cpp
  EstruturaDados/ObjetoRede/CObjetoRede_Sitio_WEST.cpp
  EstruturaDados/ObjetoRede/CObjetoRede_Final.cpp
  EstruturaDados/ObjetoRede/CObjetoRede_Tipo.cpp

  EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto.cpp
  EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Ligacao.cpp
  EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Ligacao_EST.cpp
  EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Ligacao_WEST.cpp
  EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Sitio.cpp
  EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Sitio_EST.cpp
  EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Sitio_WEST.cpp
  EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Final.cpp
  EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Tipo.cpp

  EstruturaDados/CBaseGrafoRedeEsqueleto.cpp
  EstruturaDados/CGrafo.cpp
  EstruturaDados/CRede.cpp
  EstruturaDados/CRedeContorno.cpp

  AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial.cpp
  AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M1.cpp
  AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M2.cpp
  AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M3.cpp
  AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M4.cpp
  AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M5.cpp
  AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M6_Tortuosidade.cpp
  AnaliseImagem/Caracterizacao/RedePercolacao/CRedeDePercolacao.cpp
  AnaliseImagem/Caracterizacao/RedePercolacao/CContornoRedePercolacao.cpp
  AnaliseImagem/Caracterizacao/RedePercolacao/CObjetoRedePercolacao.cpp

  AnaliseImagem/Filtro/FAmplitude/CFABDoisPicos.cpp
  AnaliseImagem/Filtro/FAmplitude/CFABEntropiaJohansen.cpp
  AnaliseImagem/Filtro/FAmplitude/CFABinario.cpp
  AnaliseImagem/Filtro/FAmplitude/CFABIterativoThrussel.cpp
  AnaliseImagem/Filtro/FAmplitude/CFABMedia.cpp
  AnaliseImagem/Filtro/FAmplitude/CFABPixelContorno.cpp
  AnaliseImagem/Filtro/FAmplitude/CFABVarianciaOtsu.cpp
  AnaliseImagem/Filtro/FAmplitude/CFACinza.cpp
  AnaliseImagem/Filtro/FAmplitude/CFAmplitude.cpp
  AnaliseImagem/Filtro/FEspacial/TCFEspacial3D.h
  AnaliseImagem/Filtro/FEspacial/CFEBorda3D.cpp
  AnaliseImagem/Filtro/FEspacial/CFEBSubstitueValor3D.cpp
  AnaliseImagem/Filtro/FEspacial/FEConectividade/CConectividade2D.cpp
  AnaliseImagem/Filtro/FEspacial/FEConectividade/CConectividade3D.cpp
  AnaliseImagem/Filtro/FEspacial/FEConectividade/TCFEConectividade3D.h
  AnaliseImagem/Filtro/FEspacial/FERotulagem/CRotulador.cpp
  AnaliseImagem/Filtro/FEspacial/FERotulagem/CRotulador2DCm.cpp
  AnaliseImagem/Filtro/FEspacial/FERotulagem/CRotuladorIRA3D.cpp
  AnaliseImagem/Filtro/FEspacial/FERotulagem/TCRotulador2D.h
  AnaliseImagem/Filtro/FEspacial/FERotulagem/TCRotulador3D.h
  AnaliseImagem/Filtro/FEspacial/TCFEspacial.h
  AnaliseImagem/Filtro/FEspacial/CFEBartlet.cpp
  AnaliseImagem/Filtro/FEspacial/CFEDeteccaoContornoMasc.cpp
  AnaliseImagem/Filtro/FEspacial/CFEGaussiano.cpp
  AnaliseImagem/Filtro/FEspacial/TCFEPassaAlta.h
  AnaliseImagem/Filtro/FEspacial/CFEBorda.cpp
  AnaliseImagem/Filtro/FEspacial/CFEEliminaBorda.cpp
  AnaliseImagem/Filtro/FEspacial/CFELaplaciano.cpp
  AnaliseImagem/Filtro/FEspacial/TCFEPassaBaixa.h
  AnaliseImagem/Filtro/FEspacial/CFEBSubstitueValor.cpp
  AnaliseImagem/Filtro/FEspacial/CFEEmboss.cpp
  AnaliseImagem/Filtro/FEspacial/CFEPassaAlta2.cpp
  AnaliseImagem/Filtro/FEspacial/CFERotacaoVerticalEDiferenca.cpp
  AnaliseImagem/Filtro/FEspacial/CFESobel.cpp
  AnaliseImagem/Filtro/FEspacial/FEDeteccaoContorno/CFEDCCanny.cpp
  AnaliseImagem/Filtro/FEspacial/FEDeteccaoContorno/CFEDCMarrHildreth.cpp
  AnaliseImagem/Filtro/FEspacial/FEDeteccaoContorno/CFEDCShenCastan.cpp
  AnaliseImagem/Filtro/FEspacial/FEDeteccaoContorno/CFEDeteccaoContorno.cpp
  AnaliseImagem/Filtro/FEspacial/FEEsqueleto/CFEEsqueleto.cpp
  AnaliseImagem/Filtro/FEspacial/FEEsqueleto/CFEEsqueletoV1.cpp
  AnaliseImagem/Filtro/FEspacial/FEEsqueleto/CFEEsqueletoV2.cpp
  AnaliseImagem/Filtro/FEspacial/FEEsqueleto/CFEEsqueletoV3.cpp
  AnaliseImagem/Filtro/FEspacial/FEEsqueleto/CFEEsqueletoV4.cpp
#  AnaliseImagem/Filtro/FEspacial/FEEsqueleto/TFEEsqueleto3D.cpp
#  AnaliseImagem/Filtro/FEspacial/FEEsqueleto/TFEEsqueletoV13D.cpp
#  AnaliseImagem/Filtro/FEspacial/FEEsqueleto/TFEEsqueletoV23D.cpp
#  AnaliseImagem/Filtro/FEspacial/FEEsqueleto/TFEEsqueletoV33D.cpp
#  AnaliseImagem/Filtro/FEspacial/FEEsqueleto/TFEEsqueletoV43D.cpp
  AnaliseImagem/Filtro/FEspacial/FEEsqueleto/CFEEZhangSuen.cpp
  AnaliseImagem/Filtro/FEspacial/FEInversao/CFEInversao.cpp
  AnaliseImagem/Filtro/FEspacial/FEInversao/CFEInversao3D.cpp
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica3D.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDF.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDF3D.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdj.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdijk.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdijk3D.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdij.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdi.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFEuclidiana.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFEuclidiana3D.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd34.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd3453D.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd4.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd5711.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd8.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIRA.h
  AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIRA3D.h
  AnaliseImagem/Filtro/FFrequencial/FFT/CFFT.cpp
  AnaliseImagem/Filtro/FFrequencial/FFT/CFFTPropriedades.cpp
  AnaliseImagem/Filtro/Mascara/CMascara.cpp
  AnaliseImagem/Filtro/Mascara/CMascara3D.cpp
  AnaliseImagem/Filtro/Mascara/CMBartlet.cpp
  AnaliseImagem/Filtro/Mascara/CMDeteccaoContorno.cpp
  AnaliseImagem/Filtro/Mascara/CMEmboss.cpp
  AnaliseImagem/Filtro/Mascara/CMGaussiano.cpp
  AnaliseImagem/Filtro/Mascara/CMLaplaciano.cpp
  AnaliseImagem/Filtro/Mascara/CMPAssaAlta.cpp
  AnaliseImagem/Filtro/Mascara/CMPassaAlta2.cpp
  AnaliseImagem/Filtro/Mascara/CMPassaBaixa.cpp
  AnaliseImagem/Filtro/Mascara/CMPassaBaixa3d.cpp
  AnaliseImagem/Filtro/Mascara/CMRotacaoVerticaleDiferenca.cpp
  AnaliseImagem/Filtro/Mascara/CMSobelHorizontal.cpp
  AnaliseImagem/Filtro/Mascara/CMSobelVertical.cpp
  AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCDiscreta.cpp
  AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCd4.cpp
  AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCd34.cpp
  AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCd5711.cpp
  AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCd8.cpp
  AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCdi.cpp
  AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCdij.cpp
  AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCdijk.cpp
  AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCEuclidiana.cpp
  AnaliseImagem/Filtro/TCFiltro.h
  AnaliseImagem/Filtro/TCFiltro3D.h
  
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdi.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdj.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdij.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdijk.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd4.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd8.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd34.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd5711.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCDiscreta.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCEuclidiana.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd3453D.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdijk3D.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCDiscreta3D.cpp
  AnaliseImagem/Geometria/Bola/BCDiscreta/CBCEuclidiana3D.cpp
  
  #AnaliseImagem/Matriz/CImg.cpp
  #AnaliseImagem/Matriz/TCImagem3D.h
  #AnaliseImagem/Matriz/CImagem.cpp
  #AnaliseImagem/Matriz/CHistograma.cpp
  #AnaliseImagem/Matriz/CVetor.cpp
  #AnaliseImagem/Matriz/TCMatriz2D.h
  #AnaliseImagem/Matriz/TCMatriz3D.h
  #AnaliseImagem/Matriz/CBaseMatriz.cpp
  #AnaliseImagem/Matriz/CMatrizObjetoImagem.cpp
  AnaliseImagem/Matriz/CMatrizObjetoRede.cpp

  AnaliseImagem/Reconstrucao/CFReconstrucao.cpp
  AnaliseImagem/Reconstrucao/CReconstrucao.cpp
  AnaliseImagem/Reconstrucao/CReconstrucaoBueno.cpp
  AnaliseImagem/Reconstrucao/CReconstrucaoEsferas.cpp
  AnaliseImagem/Reconstrucao/CReconstrucaoZhirong.cpp
  
  AnaliseImagem/Segmentacao/PorosGargantas/CAberturaDilatacao.cpp
  AnaliseImagem/Segmentacao/PorosGargantas/CAberturaDilatacaoOld.cpp
  AnaliseImagem/Segmentacao/PorosGargantas/CAberturaDilatacao3D.cpp
  AnaliseImagem/Segmentacao/PorosGargantas/CObjetoImagem.cpp
  AnaliseImagem/Segmentacao/PorosGargantas/CSegPorosGargantas3D.cpp
  
  AnaliseImagem/Simulacao/ConfiguracaoEquilibrio/CConfiguracoesEquilibrio2D.cpp
  AnaliseImagem/Simulacao/ConfiguracaoEquilibrio/CConfiguracoesEquilibrio3D.cpp
  AnaliseImagem/Simulacao/ConfiguracaoEquilibrio/CCamara.cpp
  AnaliseImagem/Simulacao/ConfiguracaoEquilibrio/CCamara2D.cpp
  AnaliseImagem/Simulacao/ConfiguracaoEquilibrio/CCamara3D.cpp
  AnaliseImagem/Simulacao/Permeabilidade/CPermeabilidadeIntrinsecaByRede.cpp
  AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidade.cpp
  AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidadeIntrinseca.cpp
  AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidadeRelativa.cpp
  AnaliseImagem/Simulacao/Permeabilidade/GrafoConexaoSerial/CSimPermeabilidadeGrafo.cpp
  AnaliseImagem/Simulacao/Permeabilidade/RedePercolacao/CSimPermeabilidadeRede.cpp
  AnaliseImagem/Simulacao/Tortuosidade/CTortuosidade.cpp

  Base/CSolver.cpp
  Base/CSimulacao.cpp
  Base/COperacao.cpp
  Base/CMath.cpp
  Base/CFuncao.cpp
  
  Estatistica/CERandomico.cpp
  Estatistica/CEstatistica.cpp
  Estatistica/CEFatorial.cpp
  Estatistica/CECombinacoes.cpp
  Estatistica/CEMomentos.cpp
  Estatistica/CENormal.cpp
  Estatistica/CETStudent.cpp
  Estatistica/CEChiQuadrado.cpp
  Estatistica/CEFSnedecor.cpp
  Estatistica/CEIntervalosConfianca.cpp
  Estatistica/CETesteMediaDuasAmostras.cpp
  
  Grafico/Gnuplot/CGnuplot.cpp
  
  MetNum/Contorno/CContornoCentro.cpp
  MetNum/Contorno/CContorno.cpp
  
  MetNum/Funcao/CFChebyshev.cpp
  MetNum/Funcao/CFGamma.cpp
  MetNum/Funcao/CFLegendre.cpp
  MetNum/Funcao/CFPolinomio.cpp
  MetNum/Funcao/CFBesselJ.cpp
  MetNum/Funcao/CFCi.cpp
  MetNum/Funcao/CFHermite.cpp
  MetNum/Funcao/CFNaoLinearRzRy.cpp
  MetNum/Funcao/CFSi.cpp
  MetNum/Funcao/CFBesselY.cpp
  MetNum/Funcao/CFEi.cpp
  MetNum/Funcao/CFIterativa.cpp
  MetNum/Funcao/CFPHermite.cpp
  MetNum/Funcao/CFSomatorioCm2RyM.cpp
  MetNum/Funcao/CFBeta.cpp
  MetNum/Funcao/CFErf.cpp
  MetNum/Funcao/CFLaguerre.cpp
  MetNum/Funcao/CFPHermiteRec.cpp
  MetNum/Funcao/CFuncoesEspeciais.cpp
  
  MetNum/Integral/CIGauss.cpp
  MetNum/Integral/CIGHermiteQuadrature.cpp
  MetNum/Integral/CIGLegendreQuadrature.cpp
  MetNum/Integral/CIGChebyshevQuadrature.cpp
  MetNum/Integral/CIGLaguerreQuadrature.cpp
  MetNum/Integral/CIntegral.cpp
  MetNum/Integral/CISimpson.cpp
  
  MetNum/Interpolacao/CIntLagrange.cpp
  MetNum/Interpolacao/CINewtonDiff.cpp
  MetNum/Interpolacao/CINewtonDivDiff.cpp
  MetNum/Interpolacao/CIntBarycentric.cpp
  MetNum/Interpolacao/CIntCubSpline.cpp
  MetNum/Interpolacao/CIntEDBarycentric.cpp
  MetNum/Interpolacao/CInterpolacao.cpp
  MetNum/Interpolacao/IntMetNum/CIntMetNum.cpp
  MetNum/Interpolacao/IntMetNum/CIMNDiferencasCentrais.cpp
  MetNum/Interpolacao/IntMetNum/CIMNExata.cpp
  MetNum/Interpolacao/IntMetNum/CIMNUpwind.cpp
  MetNum/Interpolacao/IntMetNum/CIMNWuds.cpp
  MetNum/Interpolacao/Interpolacao2D/CInterpolacao2D.cpp
  MetNum/Interpolacao/Interpolacao2D/CInt2DBilinear.cpp
  
  MetNum/Matriz/CImg.cpp
  MetNum/Matriz/TCImagem3D.h
  MetNum/Matriz/CImagem.cpp
  MetNum/Matriz/CHistograma.cpp
  MetNum/Matriz/CVetor.cpp
  MetNum/Matriz/TCMatriz2D.h
  MetNum/Matriz/TCMatriz3D.h
  MetNum/Matriz/CBaseMatriz.cpp
  MetNum/Matriz/CMatrizObjetoImagem.cpp

  MetNum/Solver/Funcao/CSFBissecao.cpp
  MetNum/Solver/Funcao/CSFCordas.cpp
  MetNum/Solver/Funcao/CSFuncao.cpp
  MetNum/Solver/Funcao/CSFNewton.cpp
  MetNum/Solver/Funcao/CSFRichmond.cpp

  MetNum/Solver/SistemaEquacoes/CSolverMatriz.cpp
  MetNum/Solver/SistemaEquacoes/CSolverMatriz_ParametroSolver.cpp
  MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_GaussSeidel.cpp
  MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_SOR.cpp
  MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonalDominante.cpp
  MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_Jacobi.cpp
  MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_Threads.cpp

  Tempo/CTime.cpp
)
