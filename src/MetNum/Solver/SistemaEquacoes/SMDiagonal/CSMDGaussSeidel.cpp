/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:
		Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:     André Duarte Bueno
@file:       CSMDGaussSeidel.cpp
@begin:      Sat Sep 16 2000
@copyright:  (C) 2000 by André Duarte Bueno
@email:      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDGaussSeidel.h>

using namespace std;

// ---------------------------------------------------------------------------
// Funcao AtualizaX
// ---------------------------------------------------------------------------
// A classe CSMDJacobi redefine a funcao AtualizaX
// Solução para GaussSeidel.
//  X[k]=obj[k]->Go();
//  *oj[k]=X[k];
void CSMDGaussSeidel::AtualizaX ()
{
  // Faz o mesmo que a classe CSMDiagonalDominante,
  // calcula novo valor de x e armazena em X[k].
  vx[k] = ((*obj)[k])->Go ();

  // Mas, diferente de CSMDiagonalDominante, já atualiza o valor da variável no próprio objeto k.
  // *(*obj)[k] retorna um objeto CSMParametroSolver que tem sobrecarga para receber um double
  (*obj)[k]->x = vx[k]; //*((*obj)[k]) = vx[k];
}

// ----------------------------------------------------------------------------
// Sobrecarga operador saida stream
// ----------------------------------------------------------------------------
// Escreve os atributos do objeto na ostream (saida para disco)
// ostream & operator<< (ostream & os, const CSMDGaussSeidel & s)
// {
//   os << s.limiteIteracoes << '\n'
//      << s.iteracoes << '\n'
// 	 << s.limiteErro << '\n'
// 	 << s.erro << '\n'
// 	 << s.erroMedio << '\n'
// //   std::vector < CSMParametroSolver * >* obj{ nullptr }; 
// //   unsigned long int dimensaoVetor{0};
// //   long double *vx { nullptr };
//   return os;
// }

// ----------------------------------------------------------------------------
// Sobrecarga operador entrada streams
// ----------------------------------------------------------------------------
// Lê os dados do objeto da istream
// istream & operator>> (istream & is, CSMDGaussSeidel & s)
// {
//   is >> s.limiteIteracoes
//      >> s.iteracoes 
//      >> s.limiteErro;
// 	 >> s.erro
// 	 >> s.erroMedio ;
// //   std::vector < CSMParametroSolver * >* obj{ nullptr }; 
// //   unsigned long int dimensaoVetor{0};
// //   long double *vx { nullptr };
//   return is;
// }

