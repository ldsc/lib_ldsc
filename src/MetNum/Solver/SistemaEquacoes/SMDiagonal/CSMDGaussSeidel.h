#ifndef CSMDGaussSeidel_h
#define CSMDGaussSeidel_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSMDGaussSeidel...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CSMDGaussSeidel.h
@begin      Sun Sep 17 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
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
#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDiagonalDominante.h>
#endif

/**
 * @brief Implementa o solver de Gauss Seidel.
 * O solver Gauss_Seidel é semelhante ao Jacobi, a diferenca é que ao calcular o valor de vx 
 * para a linha k utiliza o valor de vx[k] já calculado no objeto (implicito).
 * Deste modo o sistema, na maioria dos casos, converge mais rapidamente.
 * @author André Duarte Bueno
*/
class CSMDGaussSeidel : public CSMDiagonalDominante
{
// --------------------------------------------------------------Atributos
public:

// -------------------------------------------------------------Construtor
  /// Construtor, Recebe os parâmetros limite de erro e de iterrações
CSMDGaussSeidel (unsigned long int _limiteIteracoes, long double _limiteErro/*, unsigned long int _size = 0*/)
: CSMDiagonalDominante (_limiteIteracoes, _limiteErro/*,_size*/)  {
  }

// --------------------------------------------------------------Destrutor
/// Destrutor
    virtual ~ CSMDGaussSeidel () = default;

// ----------------------------------------------------------------Métodos
protected:

  /// Função que atualiza os valores de X,herdada
  virtual void AtualizaX ();

// -------------------------------------------------------------Sobrecarga

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//  friend std::ostream& operator<< (std::ostream& os, CSMDGaussSeidel& obj);
//  friend std::istream& operator>> (std::istream& is, CSMDGaussSeidel& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// std::ostream& operator<< (std::ostream& os, CSMDGaussSeidel& obj);
// std::istream& operator>> (std::istream& is, CSMDGaussSeidel& obj);

#endif
