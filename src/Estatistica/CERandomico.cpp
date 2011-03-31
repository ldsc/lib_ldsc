
// ---------------------------------------------------------------------------
// Bibliotecas   .
// ---------------------------------------------------------------------------
#include <cmath>
#include <iostream>
using namespace std;

// ---------------------------------------------------------------------------
// Bibliotecas LDSC
// ---------------------------------------------------------------------------
#include <Estatistica/CERandomico.h>	// inclue a classe


// Função que gera um número aleatório.
// float CERandomico::Go()
// Para ser herdeira de TFuncao recebe x=0 e y=0
double
CERandomico::Go (double x, double y)
{				// (intervalo+minimo+1)
  // (7,28318530718);      // 2*pi+1
  // return (random(10000+1)/10000.0)*intervalo+minimo;
  // return (rand()/(float)(RAND_MAX+1))*intervalo+minimo;        // minimo=0, intervalo=2PI

  return (ran2 (&semente)) * intervalo + minimo;	// minimo=0, intervalo=2PI
}

#define M 714025

#define IA 1366

#define IC 150889

double
CERandomico::ran2 (long *idum)
{
  static long iy, ir[98];

  static int iff = 0;

  int j;

  if (*idum < 0 || iff == 0)
    {
      iff = 1;
      if ((*idum = (IC - (*idum)) % M) < 0)
	*idum = -(*idum);
      for (j = 1; j <= 97; j++)
	{
	  *idum = (IA * (*idum) + IC) % M;
	  ir[j] = (*idum);
	}
      *idum = (IA * (*idum) + IC) % M;
      iy = (*idum);
    }

  j = 1 + 97.0 * iy / M;

  if (j > 97 || j < 1)
    std::cerr << "RAN2: This cannot happen." << std::endl;

  iy = ir[j];

  *idum = (IA * (*idum) + IC) % M;

  ir[j] = (*idum);

  return (double) iy / M;
}

#undef M

#undef IA

#undef IC


/*
// Abaixo tirado do livro Algorithins for imagem processing
//  Small system random number generator
double drand32 ()
{
	static long a=16807L, m=2147483647L,
		    q=127773L, r = 2836L;
	long lo, hi, test;

	hi = seed / q;
	lo = seed % q;
	test = a*lo -r*hi;
	if (test>0) seed = test;
	else seed = test + m;

	return (double)seed/(double)m;
}
void srand32 (long k)
{
	seed = k;
}
*/
