#ifndef CSolverMatrizDiagonalDominante_TDMA_h
#define CSolverMatrizDiagonalDominante_TDMA_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSolverMatrizDiagonalDominante_TDMA...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     Andre Duarte Bueno
@file       CSolverMatrizDiagonalDominante_TDMA.h
@begin      Sun Sep 17 2000
@copyright  (C) 2000 by Andre Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <SMatriz/SMDiagonal/CSolverMatrizDiagonalDominante.h>

// ===============================================================================
// Documentacao Classe: CSolverMatrizDiagonalDominante_TDMA
// ===============================================================================
/**
 * @brief 	Solucao do sistema de equacoes usando o metodo TDMA.
 * TDMA= Tri-Diagonal Matriz Algoritimo.
 * 
 * Resolve sistemas de equacoes para matrizes com diagonal dominante
 * com tres variáveis.
 * é um sistema direto, sem iteracao, e portanto muito rápido.
 * Entretanto é um sistema que resolve apenas problemas unidimensionais,
 * de modo que se o problema for bi ou tri-dimensional, o programa principal
 * vai ter de gerenciar a chamada do TDMA diversas vezes e controlar
 * a solucao do sistema de equacoes de uma forma iterativa.
 * 
 * Posteriormente implementar um objeto que utilize o TDMA iterativamente
 * e possa ser utilizado para resolver sistemas com objeto bi e tridimensionais
 * 
 * Obs: 
 * Segundo a hierarquia, a funcao AtualizaX deveria atualizar o valor de X.
 * Mas o TDMA usa um procedimento de calcular todos os p e q,
 * e posteriormente atualizar todos os valores de X.
 * A solução é:
 * 1- Tirar o TDMA desta hierarquia
 * 2- A funcao atualiza X faria o seguinte:
 * -Chamaria CalculaPQ (que calcularia todos os valores de p e q)
 * -Atualizaria todos os valores de X
 * -Faria k=numeroObjetosPassados (encerrando o loop).
 * @author 	Andre Duarte Bueno	
 * @see		SMatriz
*/
class CSolverMatrizDiagonalDominante_TDMA : public CSolverMatrizDiagonalDominante
{
// --------------------------------------------------------------Atributos
protected:
	double *p;	///< Variáveis auxiliares
	double *q;	///< Variáveis auxiliares

public:
// -------------------------------------------------------------Construtor
    /// Construtor
	CSolverMatrizDiagonalDominante_TDMA ();

// --------------------------------------------------------------Destrutor
    /// Destrutor
	virtual ~ CSolverMatrizDiagonalDominante_TDMA ();

// ----------------------------------------------------------------Métodos
protected:
	/// Funcao que atualiza os valores de X
    virtual void AtualizaX ();	

  // Calcula os valores de p e q
  // void CalculaPQ();
public:
  // Calcula e retorna a solução X do sistema
  // virtual TVetor* Go(TMatriz2D* A=0,TVetor* B=0){};                     

// -------------------------------------------------------------Sobrecarga

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CSolverMatrizDiagonalDominante_TDMA& obj);
//       friend istream& operator>> (istream& is, CSolverMatrizDiagonalDominante_TDMA& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funcoes Friend
// ostream& operator<< (ostream& os, CSolverMatrizDiagonalDominante_TDMA& obj);
// istream& operator>> (istream& is, CSolverMatrizDiagonalDominante_TDMA& obj);

#endif
