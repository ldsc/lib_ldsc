


#include <cmath>

#include <Funcao/CFErf.h>

// nao usa y
double
CFErf::Go (double x, double y)
{
  double t = 1 / (1 + 0.32759 * x);

  double a[6] = { 0, 0.254829592, -0.284496736,
    1.424143741, -1.453152027, 1.061405429
  };

  double sum = 0;

  for (int i = 5; i > 0; i--)
    sum = (sum + a[i]) * t;

  return 1 - sum / exp (x * x);
}
