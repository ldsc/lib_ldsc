

#include <cmath>

#include <Funcao/CFEi.h>

// nao usa y
double
CFEi::Go (double x, double y)
{
  const double tolerance = 1.e-8;

  double sum = 0;

  double term;

  double pow = x;

  double fact = 1;

  int n = 1;

  do
    {
      term = pow / n / fact;
      sum += term;

      /* update components of the next term */
      n++;
      fact *= (double) n;
      pow *= x;

    }
  while (fabs (term) > tolerance);

  return 0.5772156649 + log (x) + sum;
}
