/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
			[LDSC].
@author:          André Duarte Bueno
File:             CSMDGaussSeidel.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by André Duarte Bueno
email:            andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <SMatriz/SMDiagonal/CSMDGaussSeidel.h>

using namespace std;
// ---------------------------------------------------------------------------
// Funcao AtualizaX
// ---------------------------------------------------------------------------
// A classe CSMDJacobi redefine a funcao AtualizaX
// Solução para GaussSeidel.
//                       X[k]=obj[k]->Go();
//          *oj[k]=X[k];
// double         CSMDGaussSeidel::AtualizaX()
void
CSMDGaussSeidel::AtualizaX ()
{
  // Go calcula e retorna o valor atual da variavel
  // (*obj) retorna um objeto vetor
  // (*obj)[k] retorna um elemento do objeto vetor,
  // que é um ponteiro para CSMParametroSolver que tem a função Go
  // que retorna o valor de x do objeto k atualizado
  X[k] = ((*obj)[k])->Go ();

  // Atualiza a variável no objeto
  // *(*obj)[k] retorna um objeto CSMParametroSolver que tem sobrecarga para receber um double
  *((*obj)[k]) = X[k];
}
