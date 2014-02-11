

#include <MetNum/Interpolacao/CIntLagrange.h>

// Foi definida como estatica
double CIntLagrange::Lagrange (double *xarr, double *yarr, int n, double x)
{
  double prod, yint = 0;
  int i, j;

  //  loop for each term
  for (i = 0; i < n; i++)
    {
      // initialize term with yint[i]
      prod = *(yarr + i);
      // build each term
      for (j = 0; j < n; j++)
	if (i != j)
	  prod *= (x - *(xarr + j)) / (*(xarr + i) - *(xarr + j));

      yint += prod;
    }
  return yint;
}
