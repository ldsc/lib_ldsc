
#include <cmath>

#include <Estatistica/CEFatorial.h>

#include <Funcao/CFCi.h>

// usa fatorial
// nao usa y
double
CFCi::Go (double x, double y)
{
  const double tolerance = 1.e-8;

  double sum = 0;

  double term;

  double pow = x * x;

  double chs = -1;

  int n = 1;

  CEFatorial fatorial (50);	// o valor 50 precisa ser definido de acordo com a frequencia de uso

  do
    {
      term = chs * pow / (2 * n) / fatorial (2 * n);
      sum += term;

      /* update components of the next term */
      n++;
      chs = -chs;
      pow *= x * x;

    }
  while (fabs (term) > tolerance);

  return 0.5772156649 + log (x) + sum;
}
