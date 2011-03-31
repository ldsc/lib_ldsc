#include <stdlib.h>
#include <math.h>
#include "global.h"
/*
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define TRUE 1
#define FALSE 0
#define EPSILON 1.0e-15
#define BAD_RESULT -1.0e+30
#define MISSING_DATA -2.0e+30

#define SQR(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))
#define ONEOVER(x) (1 / (x))
#define RECIPROCAL(x) (1 / (x))

typedef int Boolean;

enum boolean { false, true };
enum answer { no, yes, dont_care };

#endif
*/
#include "interp.h"

double Lagrange(double* xarr, double* yarr, int n, double x)
{
  double prod, yint = 0;
  int i, j;

  /*  loop for each term */
  for (i = 0; i < n; i++) {
  /* initialize term with yint[i] */
    prod = *(yarr + i);
    /* build each term */
    for (j = 0; j < n; j++)
      if (i != j)
        prod *= (x - *(xarr + j)) /
	        (*(xarr + i) - *(xarr + j));

      yint += prod;
  }
  return yint;
}

/* Henrici  pages 237-238 */

double Barycentric(double* xarr, double* yarr, double* wt,
                   int calcWtFlag, int n, double x)
{
  double prod, diff, sum1 = 0, sum2 = 0;
  int i, j;

  if (calcWtFlag) {
    for (i = 0; i < n; i++) {
      prod = 1;
      for (j = 0; j < n; j++)
        if (i != j)
	  prod *= (*(xarr + i) - *(xarr + j));
	*(wt + i) = 1 / prod;
    }
  }

  /*  loop for each term */
  for (i = 0; i < n; i++) {
    diff = *(wt + i) / (x - *(xarr + i));
    sum1 += *(yarr + i) * diff;
    sum2 += diff;
  }
  return sum1 / sum2;
}

/* Henrici  page 240 flowchart */

double ED_Barycentric(double* xarr, double* yarr,
                      int n, double x)
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

  for (;;) {
    diff = x - x0;
    if (fabs(diff) < INTERP_EPS)
      diff = INTERP_EPS;
    t += w / diff;
    s += w * *(yarr + i) / diff;
    if (m != 0) {
      w *= (double)m--;
      k--;
      w /= (double)k;
      x0 += h;
      i++;
    }
    else
      break;
  }
  return s / t;
}

/* see Kohn, page 140 */

double NewtonDivDiff(double* xarr, double* yarr,
                     double* diffTable, int buildMatFlag,
		     int n, double x)
{
  double yint;
  int i, j;

  if (buildMatFlag) {
    for (i = 0; i < n; i++)
      *(diffTable + i) = *(yarr + i);

    for (i = 0; i < (n - 1); i++) {
      for (j = n - 1; j > i; j--)
        *(diffTable + j) = (*(diffTable + j - 1) -
	                   *(diffTable + j)) /
			   (*(xarr + j - 1 - i) - *(xarr + j));
    }
  }

  /* initialize interpolated value */
  yint = *(diffTable + n - 1);

  for (i = n - 2; i >= 0; i--)
    yint = *(diffTable + i) + (x - *(xarr + i)) * yint;

  return yint;
}

double NewtonDiff(double x0, double h, double* yarr,
                  double* diffTable, int buildMatFlag,
		  int n, double x)
{
  double yint;
  int i, j;

  if (buildMatFlag) {
    for (i = 0; i < n; i++)
      *(diffTable + i) = *(yarr + i);

    for (i = 0; i < (n - 1); i++) {
      for (j = n - 1; j > i; j--)
        *(diffTable + j) = (*(diffTable + j - 1) -
	                   *(diffTable + j)) /
			   (-h *(1 + i));
    }
  }

  /* initialize interpolated value */
  yint = *(diffTable + n - 1);

  for (i = n - 2; i >= 0; i--)
    yint = *(diffTable + i) + (x - (x0 + i * h)) * yint;

  return yint;
}

int getDerivatives(double* xarr, double* yarr, int n,
                   double* deriv, double* h,
		   double tolerance)
{
  double* a;
  double* b;
  double* c;
  int i, j, k, result;

  a = (double*)malloc(n * sizeof(double));
  b = (double*)malloc(n * sizeof(double));
  c = (double*)malloc(n * sizeof(double));

  for (i = 1; i < n; i++) {
    j = i - 1;
    *(h + i) = *(xarr + i) - *(xarr + j);
    *(deriv + i) = (*(yarr + i) - *(yarr + j)) / *(h + i);
  }

  for (i = 1; i < (n - 1); i++) {
    j = i - 1;
    k = i + 1;
    *(b + j) = 2;
    *(c + j) = *(h + k)/(*(h + i) + *(h + k));
    *(a + j) = 1 - *(c + j);
    *(deriv + j) = 6 * (*(deriv + k) - *(deriv + i)) /
                   (*(h + i) + *(h + k));
  }

  /* solve for the second derivatives */
  result = tridiagonal(n - 2, a, b, c, deriv, tolerance);

  /* deallocate the dynamic arrays */
  free(a);
  free(b);
  free(c);

  return result;
}

double Spline(double* xarr, double* yarr, double* deriv,
              double* h, int n, double x)
{
  int j, i = 0;
  double delta1, delta2, y;

  /* locate subinterval containing x */
  while ((x < *(xarr + i) ||
    x > *(xarr + i + 1)) &&
    i < (n - 1))
    i++;

  if (i >= (n - 1))
    return BAD_RESULT;

  j = i + 1;
  delta1 = x - *(xarr + i);
  delta2 = *(xarr + j) - x;
  y = (*(deriv + i - 1) * delta2 * delta2 * delta2) /
      (6 * *(h + j)) +
      (*(deriv + i) * delta1 * delta1 * delta1) /
      (6 * *(h + j)) +
      (*(yarr + j) / *(h + j) - *(deriv + i)* *(h + j) / 6) *
      delta1 +
      (*(yarr + i) / *(h + j) - *(deriv + i - 1) * *(h + j) / 6) *
      delta2;

  return y;
}

int tridiagonal(int n, double* a, double* b, double* c,
                double* d, double epsilon)
{
  int isSingular;
  int i;

  isSingular = (*b < epsilon) ? TRUE : FALSE;

  /* carry out LU factorization */
  for (i = 1; i < n && !isSingular; i++) {
    *(a + i) = *(a + i) / *(b + i - 1);
    *(b + i) = *(b + i) - *(a +i) * *(c + i - 1);
    /* determine if diagonal element is too small */
    isSingular = (*(b + i) < epsilon) ? TRUE : FALSE;
    *(d + i) = *(d + i) - *(a + i) * *(d + i - 1);
  }

  if (!isSingular) {
    /* carry out backward substitution */
    *(d + n - 1) = *(d + n - 1) / *(b + n - 1);
    for (i = n - 2; i >= 0; i--)
      *(d + i) = (*(d + i) - *(c + i) * *(d + i + 1)) / *(b + i);
    return TRUE;
  }
  else
    return FALSE;
}

