
#include <cmath>

#include <MetNum/Interpolacao/CIntEDBarycentric.h>


/* Henrici  page 240 flowchart */
//foi definida como estatica
double
CIntEDBarycentric::ED_Barycentric (double *xarr, double *yarr, int n,
				   double x)
{
  double h, x0, diff, w, s, t;
  int i, k, m;

  i = 0;
  w = 1;
  k = 0;
  m = n - 1;
  s = 0;
  t = 0;
  x0 = *xarr;
  h = *(xarr + 1) - x0;

  for (;;)
    {
      diff = x - x0;
      if (fabs (diff) < INTERP_EPS)
	diff = INTERP_EPS;
      t += w / diff;
      s += w * *(yarr + i) / diff;
      if (m != 0)
	{
	  w *= (double) m--;
	  k--;
	  w /= (double) k;
	  x0 += h;
	  i++;
	}
      else
	break;
    }
  return s / t;
}
