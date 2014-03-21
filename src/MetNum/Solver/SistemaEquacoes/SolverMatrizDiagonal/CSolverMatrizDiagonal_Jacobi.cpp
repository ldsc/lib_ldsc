/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================
Desenvolvido por:
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:    André Duarte Bueno
<<<<<<< HEAD:src/MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDJacobi.cpp
@file:      CSMDJacobi.cpp
=======
@file:      CSolverMatrizDiagonal_Jacobi.cpp
>>>>>>> renomeada hierarquia solvers matriz diagonal; criado @defgroup HCSolver para doxygen.:src/MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_Jacobi.cpp
@begin:     Sat Sep 16 2000
@copyright: (C) 2000 by André Duarte Bueno
@email:     andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_Jacobi.h>

using namespace std;
// ---------------------------------------------------------------------------
// Definição das funções da classe CSolverJacobi
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Funcao Construtora
// ---------------------------------------------------------------------------
/*CSolverMatrizDiagonal_Jacobi::CSolverMatrizDiagonal_Jacobi():CSolverMatrizDiagonalDominante()
{
} */

// ---------------------------------------------------------------------------
// Funcao Destrutora
// ---------------------------------------------------------------------------
/*CSolverMatrizDiagonal_Jacobi::~CSolverMatrizDiagonal_Jacobi()
{
} */

// ---------------------------------------------------------------------------
// Funcao AtualizaX
// ---------------------------------------------------------------------------
// A classe CSolverMatrizDiagonal_Jacobi redefine a funcao AtualizaX
// Solução para Jacobi
//  X[k]=obj[k]->Go();    // Go calcula e retorna o valor atual da variavel
void CSolverMatrizDiagonal_Jacobi::AtualizaX()
{
   vx[k] = ( *obj )[k]->Go();
}

// ---------------------------------------------------------------------------
// Funcao AtualizaObjFinal
// ---------------------------------------------------------------------------
// Faz obj[k]=X[k];
void CSolverMatrizDiagonal_Jacobi::AtualizaObjFinal()
{
   for( unsigned int kk = 0; kk < obj->size(); kk++ ) {
         // X[kk] = fatorRelaxacao * X[kk] + fatorRelaxacaoC * (    (long double)(*((*obj)[kk]))   );
         // Atualiza a variável no objeto
         ( *obj )[kk]->x = vx[kk]; // *((*obj)[kk]) = vx[kk];
      }
}

