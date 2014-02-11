

#include <MetNum/Interpolacao/CIntCubSpline.h>

//foi definida como estatica
double CIntCubSpline::Spline (double *xarr, double *yarr,
		       double *deriv, double *h, int n, double x)
{
  int j, i = 0;
  double delta1, delta2, y;

  // locate subinterval containing x
  //Varia contador i de 0 até n-1, e verifica se x pertence ao intervalo
  while ((x < *(xarr + i) || x > *(xarr + i + 1))	// ou
	 && i < (n - 1)		// e
	 )
    i++;

  if (i >= (n - 1))
    return CMath::RESULTADO_ERRADO;
  //return BAD_RESULT;

  j = i + 1;
  delta1 = x - *(xarr + i);
  delta2 = *(xarr + j) - x;
  y = (*(deriv + i - 1) * delta2 * delta2 * delta2) /
    (6 * *(h + j))
    +
    (*(deriv + i) * delta1 * delta1 * delta1) /
    (6 * *(h + j))
    +
    (*(yarr + j) / *(h + j) - *(deriv + i) * *(h + j) / 6) *
    delta1
    + (*(yarr + i) / *(h + j) - *(deriv + i - 1) * *(h + j) / 6) * delta2;

  return y;
}

/*
Abaixo a funcao original
double CIntCubSpline::Spline(double* xarr, double* yarr,
			     double* deriv,double* h,
			     int n, double x)
{
  int j, i = 0;
  double delta1, delta2, y;

  // locate subinterval containing x
  //Varia contador i de 0 até n-1, e verifica se x pertence ao intervalo
  while (
	 (x < *(xarr + i)    ||    x > *(xarr + i + 1))	// ou
         &&    i < (n - 1)                               // e
	 )
    i++;

  if (i >= (n - 1))
    return CMath::RESULTADO_ERRADO;
  //return BAD_RESULT;

  j = i + 1;
  delta1 = x - *(xarr + i);
  delta2 = *(xarr + j) - x;
  y = (*(deriv + i - 1) * delta2 * delta2 * delta2) /
    (6 * *(h + j))
    +
    (*(deriv + i) * delta1 * delta1 * delta1) /
    (6 * *(h + j))
    +
    (*(yarr + j) / *(h + j) - *(deriv + i)* *(h + j) / 6) *
    delta1
    +
    (*(yarr + i) / *(h + j) - *(deriv + i - 1) * *(h + j) / 6) *
    delta2;

  return y;
}
*/
