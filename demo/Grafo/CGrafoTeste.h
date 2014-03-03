#ifndef CGrafoTest_h
#define CGrafoTest_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CGrafoTest...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CGrafoTest.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <string>
#include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef TCMatriz3D_h
#include <AnaliseImagem/Matriz/TCMatriz3D.h>
#endif

#ifndef CGrafo_3Dby2D_h
#include <Grafo/CGrafo_3Dby2D.h>
#endif

#ifndef CMFluido_h
#include <Amostra/Material/CMFluido.h>
#endif

#ifndef CSMDiagonalDominante_h
#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDiagonalDominante.h>
#endif

// #include <SMatriz/SMDiagonal/CSMDiagonalDominanteThreads.h> // novo

#ifndef CPermeabilidadeGrafo_h
#include <AnaliseImagem/Simulacao/Permeabilidade/GrafoConexaoSerial/CPermeabilidadeGrafo.h>

#endif

/**
 * @brief Classe de teste das diversas classes relacionadas ao grafo.
 * Usada para testar o grafo.
 * @author 	André Duarte Bueno
 * @see		Veja assunto
*/
class CGrafoTest
{
// --------------------------------------------------------------Atributos
public:

// -------------------------------------------------------------Construtor
/// Construtor
    CGrafoTest();

// --------------------------------------------------------------Destrutor

/// Destrutor
    virtual ~ CGrafoTest();

// ----------------------------------------------------------------Métodos
private:

    /// Informacoes iniciais
    void MostrarInstrucoesArquivosExternos();

    /// Nome da imagem
    std::string SolicitarNomeImagem();

    /// Modelo do grafo
    int SolicitarModeloGrafo();

    // Le imagem do disco // ok
    CMatriz3D *LerImagemDisco(std::string nomeArquivo);

    /// Solicita dados da imagem
    void SolicitarPropriedadesImagem(int &fatorAmplificacao, double &dimensaoPixel,
                                    std::string nomeArquivo);

    /// Cria um fluido
    CMFluido *CriarFluido();

    /// Cria um solver
    CSMDiagonalDominante *CriarSolver(int tipoSolver = 2);
    // CSMDiagonalDominanteThreads* CriarSolver();

    /// Cria o grafo
    CGrafo_3Dby2D *CriarGrafo (int modelo, std::string nomeArquivo);

    /// Determina o grafo
    void DeterminarGrafo(CGrafo_3Dby2D * grafo, CMatriz3D * pm3D,
                        std::string nomeArquivo);
    void DeterminarGrafo(CGrafo_3Dby2D * grafo, std::string nomeArquivo);

    /// Determina o grafo
    void SalvarGrafo(CGrafo_3Dby2D * grafo);

    /// Cria objeto permeabilidade
    CPermeabilidadeGrafo *CriarPermeabilidade
    (CMFluido * fluido, CSMDiagonalDominante * solver,
     // CSMDiagonalDominanteThreads* solver,
     CGrafo_3Dby2D * grafo,
     TCMatriz3D<int> * pm3D,
     int fatorAmplificacao,
     double dimensaoPixel);

    /// Determina a permeabilidade
    double DeterminarPermeabilidade(CPermeabilidadeGrafo * permeabilidade);

    // Funcoes privadas auxiliares
    /// Verifica se a simulacao ja terminou, vai reiniciar ou iniciar.
    bool Permeabilidade_By_ModelX_Decisao(std::string nomeArquivo, int modelo,
                                          TCMatriz3D<int> * pm3D,
                                          std::ofstream & fout);

    /// Executa a determinacao da permeabilidade para o caso especificado
    bool Permeabilidade_By_ModelX(std::string nomeArquivo, int modelo,
                                  TCMatriz3D<int> * pm3D, std::ofstream & fout,
                                  int tipoSolver = 2, bool reiniciar = false);

public:
    /// Determinacao do grafo
    bool Grafo();

    /// Determinacao da permeabilidade
    double Permeabilidade(int tipoSolver = 2);

    /// Determinacao da Permeabilidade de uma lista de imagens
    bool ALL(unsigned int argc, char *argv[]);

    /// Função que solicita a rotina a ser executada ou grafo ou permeabilidade
    void Run();
};

#endif
