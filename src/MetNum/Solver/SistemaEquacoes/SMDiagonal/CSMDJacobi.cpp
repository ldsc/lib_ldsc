/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
	[LDSC].
@author:          André Duarte Bueno
@file:             CSMDJacobi.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDJacobi.h>

using namespace std;
// ---------------------------------------------------------------------------
// Definição das funções da classe CSolverJacobi
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Funcao Construtora
// ---------------------------------------------------------------------------
/*CSMDJacobi::CSMDJacobi():CSMDiagonalDominante()
{
} */

// ---------------------------------------------------------------------------
// Funcao Destrutora
// ---------------------------------------------------------------------------
/*CSMDJacobi::~CSMDJacobi()
{
} */

// ---------------------------------------------------------------------------
// Funcao AtualizaX
// ---------------------------------------------------------------------------
// A classe CSMDJacobi redefine a funcao AtualizaX
// Solução para Jacobi
//  X[k]=obj[k]->Go();    // Go calcula e retorna o valor atual da variavel
void
CSMDJacobi::AtualizaX ()
{
  X[k] = ((*obj)[k])->Go ();
}

// ---------------------------------------------------------------------------
// Funcao AtualizaObjFinal
// ---------------------------------------------------------------------------
// Faz obj[k]=X[k];
void
CSMDJacobi::AtualizaObjFinal ()
{
  unsigned int size = obj->size ();
  for (unsigned int kk = 0; kk < size; kk++)
    {
      // X[kk] = fatorRelaxacao * X[kk] + fatorRelaxacaoC * (    (long double)(*((*obj)[kk]))   );
      // Atualiza a variável no objeto
      *((*obj)[kk]) = X[kk];
    }
}
