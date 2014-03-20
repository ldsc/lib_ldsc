#ifndef CSMDiagonalDominante_h
#define CSMDiagonalDominante_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSMDiagonalDominante...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CSMDiagonalDominante.h
@begin      Sun Sep 17 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
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
#include <MetNum/Solver/SistemaEquacoes/CSMParametroSolver.h>
#endif

/**
 * @brief Classe para solvers de matriz diagonal dominante.
 * Solução para Jacobi
 * vx[k]=obj[k]->Go();		// Go() calcula e retorna o valor atual da variável, mas não atualiza no proprio objeto.
 * Solução para GaussSeidel
 * vx[k]=obj[k]->Go();		// Go calcula e retorna o valor atual da variável.
 * *oj[k]=vx[k];            // atualiza a variável no objeto.
 * Solução para GaussSeidelSOR
 * vx[k]=obj[k]->Go(); 		// Go calcula e retorna o valor atual da variável.
 * Relaxacao(k);            // Considera o fator de relaxacao(somente se criado um CSMSOR).
 * *oj[k]=vx[k];            // atualiza a variável no objeto.
 * @author André Duarte Bueno
*/
class CSMDiagonalDominante : public CSolver
{
// --------------------------------------------------------------Atributos
protected:
  /// Vetor de ponteiros para objetos do tipo CSMParametroSolver 
  /// um CSMParametroSolver tem um método Go() que calcula nova estimativa de x.
  std::vector < CSMParametroSolver * >* obj{ nullptr }; 
  /// Dimensão do vetor.
  unsigned long int dimensaoVetor{0};
  /// Vetor de ponteiros do tipo double (novo valor de x).
  long double *vx { nullptr };
  
  /// Índice do objeto que esta sendo processado
  unsigned long int k {0};

public:
  /// Construtor, recebe o vetor de objetos o limite iterações e limite de erro.
  CSMDiagonalDominante ( /*std::vector < CSMParametroSolver * >* obj =  nullptr ,*/
						 unsigned long int _limiteIteracoes = 1000, 
						 long double _limiteErro = 1.0e-8 )
  : CSolver ( _limiteIteracoes, _limiteErro )/*, dimensaoVetor ( obj->size() )*/ {
	 // se a dimensão do vetor for maior que zero aloca o mesmo
  	if ( dimensaoVetor > 0 ) {
		vx = new long double ( dimensaoVetor );
		assert (vx);
	}
  }

  /// Destrutor.
  virtual ~ CSMDiagonalDominante () {
    if (vx)
      delete [] vx;
  }

// ----------------------------------------------------------------Métodos
protected:
  /// O método Go(), atualiza o valor de x.
  /// Os objetos armazenados em std::vector < CSMParametroSolver * >*, do tipo CSMParametroSolver,
  /// tem uma função Go() que é usada para gerar uma nova estimativa de x.
  /// O método AtualizaX () chama Go() do objeto.
  virtual void AtualizaX ()  {  }

  /// Atualiza a propriedade x no objeto.
  virtual void AtualizaObjFinal ()  {  }

public:
  /// Realiza a determinação da solução do sistema de equações.
  virtual double Go (std::vector < CSMParametroSolver * >*objeto);

// --------------------------------------------------------------------Get
  /// Retorna vetor de ponteiros para x, vx.
  long double *Vx () const  {
    return vx;
  }

  /// Retorna vetor de objetos obj.
  std::vector < CSMParametroSolver * >* Obj () const  {
    return obj;
  }
// --------------------------------------------------------------------Set
  /// Seta vetor de ponteiro para x, vx.
  void Vx (long double *_vx)  {
    vx = _vx;
  }

  /// Seta vetor de objetos, obj.
  void  Obj ( std::vector < CSMParametroSolver * >* _objeto )  {
    obj = _objeto;
  }

  /// Sobrecarga do operador <<
//   friend std::ostream & operator<< (std::ostream & os, const CSMDiagonalDominante & obj);
};

// std::ostream & operator<< (std::ostream & os, const CSMDiagonalDominante & obj);
#endif
