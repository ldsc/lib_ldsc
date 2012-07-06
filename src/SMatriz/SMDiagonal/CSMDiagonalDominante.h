#ifndef CSMDiagonalDominante_h
#define CSMDiagonalDominante_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSMDiagonalDominante...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CSMDiagonalDominante.h
@begin      Sun Sep 17 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cassert>
#include <iostream>
#include <vector>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CSolver_h
#include <Base/CSolver.h>
#endif

#ifndef CSMParametroSolver_h
#include <SMatriz/CSMParametroSolver.h>
#endif

/**
 * @brief Classe para solvers de matriz diagonal dominante.
 * @author André Duarte Bueno	
 * @see	 SMatriz
*/
class CSMDiagonalDominante:public CSolver
{
// --------------------------------------------------------------Atributos
protected:
  unsigned long int k;  ///< iteracao k
  unsigned long int dimensaoVetor; ///< Dimensao do vetor
  std::vector < CSMParametroSolver * >*obj; ///< Vetor ponteiros para objetos do tipo CSMParametroSolver 
  long double erroMedio; ///< Erro medio
  long double *X; ///< Vetor de ponteiros do tipo double

public:
	/// Construtor
  CSMDiagonalDominante ( unsigned long int _limiteIteracoes=100, long double _limiteErro = 0.00001, unsigned long int _size = 0 )
		: CSolver ( _limiteIteracoes, _limiteErro ), k (0), dimensaoVetor (_size), obj (0), erroMedio (0), X (NULL)
  {
  	if (dimensaoVetor > 0)
	{
		X = new long double (_size);
		assert (X);
		//std::cerr << "\nERRO! alocacao do vetor X em CSMDiagonalDominante::CSMDiagonalDominante()." << std::endl;
	}
  }

  /// Destrutor
  virtual ~ CSMDiagonalDominante ()
  {
    if (X)
      delete [] X;
  }
  
// ----------------------------------------------------------------Métodos
protected:
	/// Atualiza o valor de x
  virtual void AtualizaX ()
  {
  }
  
  /// Atualiza o objeto final
  virtual void AtualizaObjFinal ()
  {
  }
  
public:
	/// Realiza a determinação da solução do sistema
  virtual double Go (std::vector < CSMParametroSolver * >*objeto);

/// Retorna vetor de ponteiros para x
  long double *GetX () const
  {
    return X;
  }
  
  /// Retorna k
  unsigned long int Getk () const
  {
    return k;
  }
  
  /// Retorna erro médio
  long double GeterroMedio () const
  {
    return erroMedio;
  }
  /// Seta ponteiro para vetor x
  void SetX (long double *_X)
  {
    X = _X;
  }
  
  /// Seta k
  void Setk (unsigned long int _k)
  {
    k = _k;
  }

  /// Sobrecarga do operador <<
  friend std::ostream & operator<< (std::ostream & os,
				    const CSMDiagonalDominante & obj);
};

std::ostream & operator<< (std::ostream & os,
			   const CSMDiagonalDominante & obj);
#endif
