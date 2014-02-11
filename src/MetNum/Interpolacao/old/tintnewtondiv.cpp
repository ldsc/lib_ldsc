
#include "TInterpolacao/TIntNewtonDiv.h"


double
TIntNewtonDiv::NewtonDiff (double x0, double h, double *yarr,
			   double *diffTable, int buildMatFlag,
			   int n, double x)
{
  double yint;
  int i, j;

  if (buildMatFlag)
    {
      for (i = 0; i < n; i++)
	*(diffTable + i) = *(yarr + i);

      for (i = 0; i < (n - 1); i++)
	{
	  for (j = n - 1; j > i; j--)
	    *(diffTable + j) = (*(diffTable + j - 1) -
				*(diffTable + j)) / (-h * (1 + i));
	}
    }

  /* initialize interpolated value */
  yint = *(diffTable + n - 1);

  for (i = n - 2; i >= 0; i--)
    yint = *(diffTable + i) + (x - (x0 + i * h)) * yint;

  return yint;
}
