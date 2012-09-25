#ifndef CSMDJacobi_h
#define CSMDJacobi_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CSMDJacobi...
  ===============================================================================
  Desenvolvido por:	
  Laboratorio de Desenvolvimento de Software Cientifico 	
  [LDSC].
  @author     Andre Duarte Bueno
  @file       CSMDJacobi.h
  @begin      Sun Sep 17 2000
  @copyright  (C) 2000 by Andre Duarte Bueno
  @email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// #include "CSolverMatriz\CSMDiagonalDominante.h"
#include <Base/_LIB_LDSC_CLASS.h>
#include <SMatriz/SMDiagonal/CSMDiagonalDominante.h>

//  ===============================================================================
//   Documentacao CLASSE: CSMDJacobi
//  ===============================================================================
/**
 * @brief 	Resolve o sistema de equacoes utilizando a rotina de Jacobi.
 * 
 * Jacobi é um sistema iterativo, lento, que calcula o valor da variavel var
 * com base nos coeficientes da linha.
 * Como resolve linha a linha, é necessária a realizacao de iteracoes
 * ate que o sistema convirja.
 * Nos computadores mais novos com o processamento paralelo, voltou a ser
 * utilizado.
 * As iteracoes sao controladas por limiteIteracoes e limiteErro.
 * @author 	Andre Duarte Bueno	
 * @see		SMatriz
*/
class CSMDJacobi : public CSMDiagonalDominante
{
  // --------------------------------------------------------------Atributos
  public:

  // -------------------------------------------------------------Construtor
	  /// Construtor
  CSMDJacobi (unsigned long int _limiteIteracoes, long double _limiteErro, 
	      unsigned long int _size = 0):CSMDiagonalDominante 
    (_limiteIteracoes, _limiteErro,    _size)
    {
    }

    // --------------------------------------------------------------Destrutor
    
    /// Destrutor
    virtual ~ CSMDJacobi ()
      {
      }

    // ----------------------------------------------------------------Métodos
 protected:

    // Calcula e retorna a solucao X do sistema
    // virtual TVetor* Go(TCMatriz2D* A=0,TVetor* B=0);
    /// Funcao que atualiza os valores de X
    virtual void AtualizaX ();

    /// Faz obj[k]=X[k];
    virtual void AtualizaObjFinal ();

    // Considera fator de relaxação
    // virtual double Relaxacao(int k=0);

 public:

    // Calcula e retorna a solucao X do sistema
    // virtual TVetor* Go(TCMatriz2D* A=0,TVetor* B=0){};                   
    // -------------------------------------------------------------Sobrecarga

    // --------------------------------------------------------------------Get
    // --------------------------------------------------------------------Set
    // -----------------------------------------------------------------Friend
    //       friend ostream& operator<< (ostream& os, CSMDJacobi& obj);
    //       friend istream& operator>> (istream& is, CSMDJacobi& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funcoes Friend
// ostream& operator<< (ostream& os, CSMDJacobi& obj);
// istream& operator>> (istream& is, CSMDJacobi& obj);

#endif
