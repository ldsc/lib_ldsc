#ifndef CSolverMatriz_h
#define CSolverMatriz_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSolverMatriz...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CSolverMatriz.h
@begin      Sun Sep 17 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

// classe com operações usuais
#ifndef COperacao_h
#include <Base/COperacao.h>
#endif

// classe com funções matemáticas usuais
#ifndef CMath_h
#include <Base/CMath.h>
#endif

// Classe definição CVetor
#ifndef CVetor_h
#include <AnaliseImagem/Matriz/CVetor.h>
#endif

// Classe definição TCMatriz2D< int >
#ifndef TCMatriz2D_H
#include <AnaliseImagem/Matriz/TCMatriz2D.h>
#endif

// Clase base
#ifndef CSolver_h
#include <Base/CSolver.h>
#endif


/*
===============================================================================
Documentacao Classe: CSolverMatriz
===============================================================================
*/
/**
 * @brief Representa a abstracao de um solver para solucao de sistemas de equacoes.
 * 
 * Inicialmente, trata de dois tipos de matrizes (duas herdeiras),
 * uma para solucao de sistemas com matrizes comuns (SMComum) e outra
 * para solucao de sistemas com matrizes com diagonal dominante.
 * 
 * Exemplo de uso:
 * solver->Determinante(); 	// calcula e retorna o determinante
 * solver->determinante;  	// obtem ultimo valor do determinante
 * 
 * O determinante da matriz é uma propriedade da matriz e não do solver.
 * A própria matriz deveria dispor de métodos para sua determinacao.
 * Pode-se criar a classe TM2DOperacoesBasicas que incluiria:
 * Calculo do determinante.
 * Um ponteiro para um solver
 * Sobrecarga do operador /: ex: X=A/B;
 * que executaria:            {X=solver(A,B);}
 * 
 * O objetivo do solver é resolver o sistema de equações, que só passa
 * a existir se existe a matriz A e o vetor B.

 * @author 	André Duarte Bueno	
 * @see		SMatriz
*/
class CSolverMatriz : public CSolver
{
// --------------------------------------------------------------Atributos
protected:
  /// Enumerated com tipos de erro
  CMath::EMathError err;

  int rowSwapFlag; ///< flag para troca de linhas
  CVetor *index; ///< vetor indice

public:
  /// Determinante da matriz
  double determinante;

  /// Último tipo de solver selecionado
  static int tipoSolver;

  /// Ponteiro para ultimo objeto Solver
  static CSolverMatriz *ptr;


// -------------------------------------------------------------Construtor
  /// Construtor
    CSolverMatriz ( unsigned long int _limiteIteracoes, long double _limiteErro )
    		   : CSolver ( _limiteIteracoes, _limiteErro ), index (0), determinante (0)
  {
    // err = CMath::eTipoErro::ok; bug
  }

// --------------------------------------------------------------Destrutor
  /// Destrutor
  virtual ~ CSolverMatriz ()
  {
  }

// ----------------------------------------------------------------Métodos
protected:
	/// Método auxiliar, LUBackSubst  
  void LUBackSubst (TCMatriz2D< int > * &A, CVetor * &B);

  
  	/// Método auxiliar, LUDecomp 
  CMath::EMathError LUDecomp (TCMatriz2D< int > * &A);

public:
  /// Calcula e retorna a solução X do sistema
  virtual CVetor * Go (TCMatriz2D< int > * A = 0, CVetor * B = 0) = 0;	// CVetor é unsigned int bug

  /// Calcula o determinante
  double Determinante (TCMatriz2D< int > * &A);

  /// Inverte  a matriz
  CMath::EMathError Inversa (TCMatriz2D< int > * &A);

  /// Cria objeto solver e retorna
  static CSolverMatriz *Selecao (int tipo = 0);

// -------------------------------------------------------------Sobrecarga

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CSolverMatriz& obj);
//       friend istream& operator>> (istream& is, CSolverMatriz& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CSolverMatriz& obj);
// istream& operator>> (istream& is, CSolverMatriz& obj);

#endif
