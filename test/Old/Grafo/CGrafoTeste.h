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

#ifndef CMatriz3D_h
#include <Matriz/CMatriz3D.h>
#endif

#ifndef CGra3Dby2D_h
#include <Grafo/CGra3Dby2D.h>
#endif

#ifndef CMFluido_h
#include <Material/CMFluido.h>
#endif

#ifndef CSMDiagonalDominante_h
#include <SMatriz/SMDiagonal/CSMDiagonalDominante.h>
#endif

#include <SMatriz/SMDiagonal/CSMDiagonalDominanteThreads.h> // novo

#ifndef CPermeabilidadeGrafo_h
#include <Permeabilidade/CPermeabilidadeGrafo.h>
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
    void MostraInstrucoesArquivosExternos();

    /// Nome da imagem
    std::string SolicitaNomeImagem();

    /// Modelo do grafo
    int SolicitaModeloGrafo();

    // Le imagem do disco // ok
    CMatriz3D *LeImagemDisco(std::string fileName);

    /// Solicita dados da imagem
    void SolicitaPropriedadesImagem(int &fatorAmplificacao, double &sizePixel,
                                    std::string fileName);

    /// Cria um fluido
    CMFluido *CriaFluido();

    /// Cria um solver
    CSMDiagonalDominante *CriaSolver(int tipoSolver = 2);
    // CSMDiagonalDominanteThreads* CriaSolver();

    /// Cria o grafo
    CGra3Dby2D *CriaGrafo (int modelo, std::string fileName);

    /// Determina o grafo
    void DeterminaGrafo(CGra3Dby2D * grafo, CMatriz3D * pm3D,
                        std::string fileName);
    void DeterminaGrafo(CGra3Dby2D * grafo, std::string fileName);

    /// Determina o grafo
    void SalvaGrafo(CGra3Dby2D * grafo);

    /// Cria objeto permeabilidade
    CPermeabilidadeGrafo *CriaPermeabilidade
    (CMFluido * fluido, CSMDiagonalDominante * solver,
     // CSMDiagonalDominanteThreads* solver,
     CGra3Dby2D * grafo,
     CMatriz3D * pm3D,
     int fatorAmplificacao,
     double sizePixel);

    /// Determina a permeabilidade
    double DeterminaPermeabilidade(CPermeabilidadeGrafo * permeabilidade);

    // Funcoes privadas auxiliares
    /// Verifica se a simulacao ja terminou, vai reiniciar ou iniciar.
    bool Permeabilidade_By_ModelX_Decisao(std::string fileName, int modelo,
                                          CMatriz3D * pm3D,
                                          std::ofstream & fout);

    /// Executa a determinacao da permeabilidade para o caso especificado
    bool Permeabilidade_By_ModelX(std::string fileName, int modelo,
                                  CMatriz3D * pm3D, std::ofstream & fout,
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
