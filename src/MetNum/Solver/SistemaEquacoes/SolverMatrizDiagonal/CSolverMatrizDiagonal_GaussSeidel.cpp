/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:
		Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:     André Duarte Bueno
@file:       CSolverMatrizDiagonal_GaussSeidel.cpp
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
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_GaussSeidel.h>

// using namespace std;
// 
// // ---------------------------------------------------------------------------
// // Funcao AtualizaX
// // ---------------------------------------------------------------------------
// /** A classe CSolverMatrizDiagonal_Jacobi redefine a funcao AtualizaX, implementando a solução para GaussSeidel.
//  *  vx[k] = ((*obj)[k])->Go ();
//  * (*obj)[k]->x = vx[k]; //*((*obj)[k]) = vx[k];
//  */
// void CSolverMatrizDiagonal_GaussSeidel::AtualizaX ()
// {
//   // Faz o mesmo que a classe CSolverMatrizDiagonalDominante,
//   // calcula novo valor de x e armazena em X[k].
//   vx[k] = ((*obj)[k])->Go ();
// 
//   // Mas, diferente de CSolverMatrizDiagonalDominante, já atualiza o valor da variável no próprio objeto k.
//   // *(*obj)[k] retorna um objeto CSolverMatriz_ParametroSolver que tem sobrecarga para receber um double
//   (*obj)[k]->x = vx[k]; //*((*obj)[k]) = vx[k];
// }

// ----------------------------------------------------------------------------
// Sobrecarga operador saida stream
// ----------------------------------------------------------------------------
// Escreve os atributos do objeto na ostream (saida para disco)
// ostream & operator<< (ostream & os, const CSolverMatrizDiagonal_GaussSeidel & s)
// {
//   os << s.limiteIteracoes << '\n'
//      << s.iteracoes << '\n'
// 	 << s.limiteErro << '\n'
// 	 << s.erro << '\n'
// 	 << s.erroMedio << '\n'
// //   std::vector < CSolverMatriz_ParametroSolver * >* obj{ nullptr }; 
// //   unsigned long int dimensaoVetor{0};
// //   long double *vx { nullptr };
//   return os;
// }

// ----------------------------------------------------------------------------
// Sobrecarga operador entrada streams
// ----------------------------------------------------------------------------
// Lê os dados do objeto da istream
// istream & operator>> (istream & is, CSolverMatrizDiagonal_GaussSeidel & s)
// {
//   is >> s.limiteIteracoes
//      >> s.iteracoes 
//      >> s.limiteErro;
// 	 >> s.erro
// 	 >> s.erroMedio ;
// //   std::vector < CSolverMatriz_ParametroSolver * >* obj{ nullptr }; 
// //   unsigned long int dimensaoVetor{0};
// //   long double *vx { nullptr };
//   return is;
// }

