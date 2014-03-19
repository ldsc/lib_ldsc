/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
	[LDSC].
@author:          André Duarte Bueno
@file:             CSMDiagonalDominante.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++.
// -----------------------------------------------------------------------
#include <cmath>
#include <cassert>
#include <fstream>
#include <iomanip>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC.
// -----------------------------------------------------------------------
#include <Base/COperacao.h>

#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDiagonalDominante.h>

using namespace std;

// -----------------------------------------------------------------------
// Sobrecarga streans
// -----------------------------------------------------------------------
ostream & operator<< (ostream & os, const CSMDiagonalDominante & obj)
{
  return os;
}

// -----------------------------------------------------------------------
// Funcao Go
// -----------------------------------------------------------------------
// Obs: lembre-se que erro,limiteErro, iteracoes e limiteIteracoes foram herdados de CSolver
// Obs: A função Go recebe um vetor de objetos, vai percorrer todos os objeto, e atualizar
// as variáveis de cálculo nos objeto usando a função Go do objeto.
// double  CSMDiagonalDominante::Go(CSMParametroSolver* objeto,int _numeroobjeto)

// Ex:
// (*obj) é o vetor
// (*obj)[k] é o objeto k do vetor que é um ponteiro
// *((*obj)[k]) é o objeto apontado pelo vetor
double
CSMDiagonalDominante::Go (vector < CSMParametroSolver * >*objeto)
{
  // Recebo um ponteiro para um vetor, e tenho um ponteiro para um vetor interno,
  // necessário pois as funções internas de this, necessitam do vetor de objetos
  obj = objeto;

  if (X == nullptr || dimensaoVetor < obj->size ())
    {				// se menor, realoca X
      delete [] X;		// deleta X
      X = nullptr;			// zera
      X = new long double[obj->size ()];	// Aloca X como sendo do tamanho do vetor recebido
      assert (X);
    }
// inicio thread
  long double xk0;		// Valor de x para objeto k no instante anterior
//       long double erroPontual=10000000;
  long double erroPontual = 0;
  unsigned long int numeroObjetosUsados = 0;
  iteracoes = 0;		// Número de iterações

  // Cada passagem pelo laço do, equivale a um passo de tempo ou uma iteração
  do
    {
//               erro = 10000000;
      erro = 0;
      erroMedio = 0.0;

      // Resolve o sistema para um dado passo de tempo ou iteração
// k=iproc k+= nproc
      for (k = 0; k < obj->size (); k++)	// percorre todos os objeto
	{
	  xk0 = (long double) (*((*obj)[k]));	// valor de x[k] no instante anterior // aqui aqui só funciona se objeto tiver conversão para double!
cerr << "\nxk0 = (long double) (*((*obj)[" << k << "])) = " << xk0;
	  // Atualiza os valores do vetor X
// precisa passar o k
	  AtualizaX ();		// definida de forma diferente nas classes herdeiras
cerr << " depois AtualizaX -> xk0 = " << xk0 ;
	  // em alguns casos ja atualiza as propriedades
	  // nos objeto  CSMParametroSolver

	  // O erro é a diferença entre X atual X[k] dividido por xk0
	  // (valor absoluto)
	  if ((X[k] - xk0) != 0)	// os objetos da borda tem xk0==X[k] logo o erro = 0/0
	    {
	      erroPontual = xk0 > X[k] ? xk0 - X[k] : X[k] - xk0;
	      erroPontual /= xk0;	// erro*100 = percentual         
	      erroMedio += erroPontual;
	      numeroObjetosUsados++;
	    }
	  // if( erro > erroPontual )
	  if (erro < erroPontual)
	    erro = erroPontual;	// erro assume o maior valor de todos os pontos
	}
      erroMedio /= numeroObjetosUsados;	// (obj->size()-1);
      AtualizaObjFinal ();	// Atualiza valores finais nos objeto
      // (Necessário somente para o jacobi)
      iteracoes++;		// incrementa a iteração

      //  RunningView();
    }
  while (erroMedio > limiteErro &&	// usando erroMedio limiteIteracoes
	 iteracoes < limiteIteracoes
	 //       || iteracoes < 100// Garante que um numero mínimo de iteracoes sejam realizadas// novo
    );
  // se o erro calculado é maior que o limiteErro continua
  // se o limiteIteracoes ainda não foi atingido continua
  return erro;
}

/*
         // Solução para Jacobi
			X[k]=obj[k]->Go();// Go calcula e retorna o valor atual da variavel
                                       	// mas não atualiza no proprio objeto
         // Solução para GaussSeidel
			X[k]=obj[k]->Go();// Go calcula e retorna o valor atual da variavel
         *oj[k]=X[k];                      // atualiza a variável no objeto
         // Solução para GaussSeidelSOR
			X[k]=obj[k]->Go(); // Go calcula e retorna o valor atual da variavel
         Relaxacao(k);                       	// Considera o fator de relaxacao(somente se criado um CSMSOR)
         *oj[k]=X[k];                        	// atualiza a variável no objeto
*/



