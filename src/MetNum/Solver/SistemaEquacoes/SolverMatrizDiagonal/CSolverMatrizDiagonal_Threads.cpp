/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
                  	[LDSC].
@author:          André Duarte Bueno
@file:             CSolverMatrizDiagonalDominante_Threads.cpp
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
// Bibliotecas LIB_LDSCCSolverMatrizDiagonal_SOR.cpp
// -----------------------------------------------------------------------
#include <Base/COperacao.h>

#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_Threads.h>

using namespace std;

//aqui 2009 using namespace ost;		// common C++
/* //aqui 2009 

// -----------------------------------------------------------------------
// Atributos estáticos
long double *
  CSolverMatrizDiagonalDominante_ThreadsExec::X;
vector < CSolverMatriz_ParametroSolver * >*CSolverMatrizDiagonalDominante_ThreadsExec::obj;
long double *
  CSolverMatrizDiagonalDominante_ThreadsExec::ptr_erro;

Mutex
  CSolverMatrizDiagonalDominante_ThreadsExec::mutex_erro;
int
  CSolverMatrizDiagonalDominante_ThreadsExec::nproc = 1;
long double
  CSolverMatrizDiagonalDominante_ThreadsExec::limiteErro;
long double
  CSolverMatrizDiagonalDominante_ThreadsExec::fatorRelaxacao;
long double
  CSolverMatrizDiagonalDominante_ThreadsExec::fatorRelaxacaoC;
unsigned long int
  CSolverMatrizDiagonalDominante_ThreadsExec::limiteIteracoes;

// -----------------------------------------------------------------------
// Funcao Go
// -----------------------------------------------------------------------
double
CSolverMatrizDiagonalDominante_Threads::Go (vector < CSolverMatriz_ParametroSolver * >*objeto)
{
  obj = objeto;
  if (X == nullptr || dimensaoVetor < obj->size ())
    {
      delete [] X;
      X = nullptr;
      X = new long double[obj->size ()];
      assert (X);
    }

  // Passa atributos estaticos
  CSolverMatrizDiagonalDominante_ThreadsExec::obj = objeto;
  CSolverMatrizDiagonalDominante_ThreadsExec::X = X;

  // Cria objetos thread e executa
  CSolverMatrizDiagonalDominante_ThreadsExec *pthread = 0;
  vector <CSolverMatrizDiagonalDominante_ThreadsExec *> vthread(CSolverMatrizDiagonalDominante_ThreadsExec::nproc);
  cout << "dimensão do vetor de threads = " << vthread.size () << endl;

  for (int p = 0; p < CSolverMatrizDiagonalDominante_ThreadsExec::nproc; p++)
    {
      pthread = nullptr;
      pthread = new CSolverMatrizDiagonalDominante_ThreadsExec(p);
      if (pthread == nullptr)
	{
	  cerr << "\nfalha alocação pthread objeto CSolverMatrizDiagonalDominante_ThreadsExec " << p;
	}
      vthread[p] = pthread;
      cout << "Executando a thread p=" << p << endl;
      vthread[p]->start();
    }

  for (int pp = 0; pp < CSolverMatrizDiagonalDominante_ThreadsExec::nproc; pp++)
    {
      while (vthread[pp]->isRunning())
      	Thread::sleep (10000);	// milisegundos = 10s
      cout << "Thread pp =" << pp << " finalizada." << endl;
    }

  return erro;
}

// -----------------------------------------------------------------------
//                              PARTE PARALELA
// -----------------------------------------------------------------------
void
CSolverMatrizDiagonalDominante_ThreadsExec::run ()
{
  // Valor de x para objeto k no instante anterior
  long double xk0;
  long double erroPontual;
  long double erroThread;
  unsigned long int iteracoes = 0;

  // sistema por blocos
  int div = obj->size () / nproc;	// 0       1        2
  int min = iproc * div;	// 0,     div ,     2 div
  int max = (iproc + 1) * div - 1;	// div-1, 2div-1,   3 div-1
  cout << "iproc = " << iproc << " min=" << min << " max=" << max << endl;

  // Cada passagem pelo laço do, equivale a um passo de tempo ou uma iteração
  do
    {
      // Somente a primeira thread zera o erro
      if (iproc == 0)
	{
	  mutex_erro.enterMutex ();
	  *ptr_erro = 0.0;
	  mutex_erro.leaveMutex ();
	}
      erroThread = 0.0;

      // Resolve o sistema para um dado passo de tempo ou iteração
      // sistema intercalado
      // for (k = iproc; k < obj->size (); k += nproc)


      for (k = min; k <= max; k++)
	{
	  // valor de x[k] no instante anterior
	  xk0 = (long double) (*((*obj)[k]));
	  X[k] = ((*obj)[k])->Go ();
	  X[k] =
	    fatorRelaxacao * X[k] +
	    fatorRelaxacaoC * ((long double) (*((*obj)[k])));

	  // AtualizaObjFinal
	  *((*obj)[k]) = X[k];

	  if ((X[k] - xk0) != 0)
	    {
	      erroPontual = xk0 > X[k] ? xk0 - X[k] : X[k] - xk0;
	      erroPontual /= xk0;
	    }
	  if (erroThread < erroPontual)
	    erroThread = erroPontual;
	}

      // AtualizaObjFinal ();
      // for (k = iproc; k < obj->size (); k += nproc)
      //       *((*obj)[k]) = X[k];

      iteracoes++;
//               cout <<  iproc << ' '<< iteracoes << endl;
    }
  while (erroThread > limiteErro && iteracoes < limiteIteracoes);

  mutex_erro.enterMutex ();
  if ((*ptr_erro) < erroThread)
    *ptr_erro = erroThread;
  mutex_erro.leaveMutex ();
}

/*
void
CSolverMatrizDiagonalDominante_ThreadsExec::RunningView ()
{
  static int p = 0;
  switch (p)
    {
    case 0:
    case 4:
      cout << "\b|" << flush;
      break;
    case 1:
    case 5:
      cout << "\b/" << flush;
      break;
    case 2:
    case 6:
      cout << "\b-" << flush;
      break;
    case 3:
    case 7:
      cout << "\b\\" << flush;
      break;
    }
  p++;
  if (p >= 7)
    p = 0;
}
//aqui 2009 */

// *********************CSolverMatrizDiagonalDominante_Threads fim
/*
g++ -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__
-I/home/andre/Andre/Desenvolvimento/LIB_LDSC/lib_lmpt/include -I/usr/include/imago/ -I/usr/include/coi20/
-I/usr/local/include/cc++2 -D_GNU_SOURCE -pthread    -L/usr/local/lib  -c CSolverMatrizDiagonalDominante_Threads.cpp
*/
