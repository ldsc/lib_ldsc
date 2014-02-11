
// Funcoes com dois parametros

#include <cmath>

#include <MetNum/Funcao/CFBesselJ.h>

#include <Estatistica/CEFatorial.h>

#include <MetNum/Funcao/CFGamma.h>

// usa fatorial e gamma
// double BesselJ(double z, int v)
double
CFBesselJ::Go (double x, double y)
{

  // double z=x;

  int v = (int) y;

  const double tolerance = 1.0e-8;

  double sum = 0;

  double term;

  int i = 0;

  CEFatorial fatorial (50);

  CFGamma gamma;
  // o valor 50 acima, precisa ser definido melhor. Usar um valor normammente utilizado.
  do
    {
      term = pow (-x * x / 4, (double) i) / fatorial (i) / gamma (v + i + 1);
      i++;
    }
  while (fabs (term) > tolerance);

  return pow (x / 2, v) * sum;
}

/*
double BesselJ(double z, int v)
{
  const double tolerance = 1.0e-8;
  double sum = 0;
  double term;
  int i = 0;

  do {
	 term = pow(- z * z / 4, (double)i) /
			  factorial(i) / gamma(v + i + 1);
	 i++;
  } while (fabs(term) > tolerance);

  return pow(z / 2, v) * sum;
}
*/
