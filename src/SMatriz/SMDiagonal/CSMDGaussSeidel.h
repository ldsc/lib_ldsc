#ifndef CSMDGaussSeidel_h
#define CSMDGaussSeidel_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSMDGaussSeidel...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CSMDGaussSeidel.h
@begin      Sun Sep 17 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CMath_h
#include <Base/CMath.h>
#endif

#ifndef CSMDiagonalDominante_h
#include <SMatriz/SMDiagonal/CSMDiagonalDominante.h>
#endif

/**
 * @brief Implementa  o solver de gauss seidel.
 * 
 * O solver Gauss_Seidel é semelhante ao Jacobi, 
 * a diferenca é que ao calcular o valor de X para a linha j utiliza o 
 * valor de X[j] já calculado	no objeto (implicito).
 * Deste modo o sistema converge mais rapidamente (implicito).
 * @author 	André Duarte Bueno	
 * @see		SMatriz
*/
class CSMDGaussSeidel : public CSMDiagonalDominante
{
// --------------------------------------------------------------Atributos
public:

// -------------------------------------------------------------Construtor
  /// Construtor, Recebe os parâmetros limite de erro e de iterrações
CSMDGaussSeidel (unsigned long int _limiteIteracoes, 
		 long double _limiteErro, unsigned long int _size = 0)
: CSMDiagonalDominante (_limiteIteracoes, _limiteErro,	_size)
  {
  }

// --------------------------------------------------------------Destrutor
/// Destrutor
    virtual ~ CSMDGaussSeidel ()
  {
  }

// ----------------------------------------------------------------Métodos
protected:

  /// Função que atualiza os valores de X,herdada
  virtual void AtualizaX ();

// -------------------------------------------------------------Sobrecarga

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CSMDGaussSeidel& obj);
//       friend istream& operator>> (istream& is, CSMDGaussSeidel& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CSMDGaussSeidel& obj);
// istream& operator>> (istream& is, CSMDGaussSeidel& obj);

#endif
