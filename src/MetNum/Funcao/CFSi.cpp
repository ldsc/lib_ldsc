
#include <cmath>

#include <Estatistica/CEFatorial.h>

#include <MetNum/Funcao/CFSi.h>

// usa fatorial
// nao usa y
double
CFSi::Go (double x, double y)
{
  const double tolerance = 1.e-8;

  double sum = 0;

  double term;

  double pow = x;

  double chs = 1;

  int n = 0;

  CEFatorial fatorial (50);	// o valor 50 deve ser definido por frequencia de uso
  do
    {
      term = chs * pow / (2 * n + 1) / fatorial (2 * n + 1);

      sum += term;

      /* update components of the next term */
      n++;

      chs = -chs;

      pow *= x * x;
    }
  while (fabs (term) > tolerance);

  return sum;
}
