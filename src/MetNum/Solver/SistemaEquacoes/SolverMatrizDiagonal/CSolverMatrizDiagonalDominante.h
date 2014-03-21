#ifndef CSolverMatrizDiagonalDominante_h
#define CSolverMatrizDiagonalDominante_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSolverMatrizDiagonalDominante...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CSolverMatrizDiagonalDominante.h
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

#ifndef CSolverMatriz_ParametroSolver_h
#include <MetNum/Solver/SistemaEquacoes/CSolverMatriz_ParametroSolver.h>
#endif

/**
 * @brief Classe para solvers de matriz diagonal dominante.
 * Solução para Jacobi
 * vx[k]=obj[k]->Go();		// Go() calcula e retorna o valor atual da variável, mas não atualiza no proprio objeto.
 * 
 * Solução para GaussSeidel
 * vx[k]=obj[k]->Go();		// Go() calcula e retorna o valor atual da variável, mas não atualiza no proprio objeto.
 * oj[k].x=vx[k];           // atualiza a variável x no objeto.
 * 
 * Solução para GaussSeidelSOR
 * vx[k]=obj[k]->Go(); 		// Go() calcula e retorna o valor atual da variável, mas não atualiza no proprio objeto.
 * Relaxacao(k);            // Considera o fator de relaxação(somente se criado um CSMSOR).
 * oj[k].x=vx[k];           // atualiza a variável no objeto.
 * 
 * @ingroup HCSolver
 * @author André Duarte Bueno
*/
class CSolverMatrizDiagonalDominante : public CSolver
{
// --------------------------------------------------------------Atributos
protected:
  /// Vetor de ponteiros para objetos do tipo CSolverMatriz_ParametroSolver 
  /// um CSolverMatriz_ParametroSolver tem um método Go() que calcula a nova estimativa de x.
  std::vector < CSolverMatriz_ParametroSolver * >* obj{ nullptr };

  /// Dimensão do vetor.
  unsigned long int dimensaoVetorVx{0};

  /// Vetor de ponteiros do tipo double (armazena novo valor de x).
  long double *vx { nullptr };

  /// Índice do objeto que esta sendo processado. k é herdado de CSolver.
  unsigned long int k {0};

public:
  /// Construtor, recebe o limite de iterações e o limite de erro.
  CSolverMatrizDiagonalDominante ( /*std::vector < CSolverMatriz_ParametroSolver * >* obj =  nullptr ,*/
						 unsigned long int _limiteIteracoes,
						 long double _limiteErro )
  : CSolver ( _limiteIteracoes, _limiteErro )/*, dimensaoVetor ( obj->size() )*/ {
	 // se a dimensão do vetor for maior que zero aloca o mesmo
//   	if ( dimensaoVetor > 0 ) {
// 		vx = new long double ( dimensaoVetor );
// 		assert (vx);
// 	}
  }

  /// Destrutor.
  virtual ~ CSolverMatrizDiagonalDominante () {
//     if (vx)
//       delete [] vx;
  }

// ----------------------------------------------------------------Métodos
public:
  /// Realiza a determinação da solução do sistema de equações.
  virtual double Go (std::vector < CSolverMatriz_ParametroSolver * >* objeto);

protected:
  /// O método Go(), atualiza o valor de x.
  /// Os objetos armazenados em std::vector < CSolverMatriz_ParametroSolver * >*, do tipo CSolverMatriz_ParametroSolver,
  /// tem uma função Go() que é usada para gerar uma nova estimativa de x.
  /// O método AtualizaX () chama Go() do objeto.
  virtual void AtualizaX ()  {  }

  /// Atualiza a propriedade x no objeto.
  virtual void AtualizaObjFinal ()  {  }

// --------------------------------------------------------------------Get
public:
  /// Retorna vetor de ponteiros para x, vx.
  long double *Vx () const  {
    return vx;
  }

  /// Retorna vetor de objetos obj.
  std::vector < CSolverMatriz_ParametroSolver * >* Obj () const  {
    return obj;
  }
// --------------------------------------------------------------------Set
  /// Seta vetor de ponteiro para x, vx.
  void Vx (long double *_vx)  {
    vx = _vx;
  }

  /// Seta vetor de objetos, obj.
  void  Obj ( std::vector < CSolverMatriz_ParametroSolver * >* _objeto )  {
    obj = _objeto;
  }

  /// Sobrecarga do operador <<
//   friend std::ostream & operator<< (std::ostream & os, const CSolverMatrizDiagonalDominante & obj);
};

// std::ostream & operator<< (std::ostream & os, const CSolverMatrizDiagonalDominante & obj);
#endif

