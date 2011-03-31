#include <iostream>
#include <Interpolacao/CInterpolacao.h>

int
CInterpolacao::getDerivatives (double *xarr, double *yarr, int n,
			       double *deriv, double *h, double tolerance)
{
  double *a = 0;
  double *b = 0;
  double *c = 0;
  int i, j, k, result;

  //a = (double*) malloc(n * sizeof(double));
  //b = (double*) malloc(n * sizeof(double));
  //c = (double*) malloc(n * sizeof(double));
  a = new double [n];
  b = new double [n];
  c = new double [n];
  if( c == 0 )  
	  std::cerr << "Erro alocacao memoria CInterpolacao::getDerivatives" ;
  
  for (i = 1; i < n; i++)
    {
      j = i - 1;
      *(h + i) = *(xarr + i) - *(xarr + j);
      *(deriv + i) = (*(yarr + i) - *(yarr + j)) / *(h + i);
    }

  for (i = 1; i < (n - 1); i++)
    {
      j = i - 1;
      k = i + 1;
      *(b + j) = 2;
      *(c + j) = *(h + k) / (*(h + i) + *(h + k));
      *(a + j) = 1 - *(c + j);
      *(deriv + j) = 6 * (*(deriv + k) - *(deriv + i)) /
	(*(h + i) + *(h + k));
    }

  /* solve for the second derivatives */
  result = tridiagonal (n - 2, a, b, c, deriv, tolerance);

  /* deallocate the dynamic arrays */
  //free(a);
  //free(b);
  //free(c);
  delete [] a;
  delete [] b;
  delete [] c;

  return result;
}


int
CInterpolacao::tridiagonal (int n, double *a, double *b, double *c,
			    double *d, double epsilon)
{
  int isSingular;
  int i;

  isSingular = (*b < epsilon) ? true : false;

  /* carry out LU factorization */
  for (i = 1; i < n && !isSingular; i++)
    {
      *(a + i) = *(a + i) / *(b + i - 1);
      *(b + i) = *(b + i) - *(a + i) * *(c + i - 1);

      /* determine if diagonal element is too small */
      isSingular = (*(b + i) < epsilon) ? true : false;
      *(d + i) = *(d + i) - *(a + i) * *(d + i - 1);
    }

  if (!isSingular)
    {
      /* carry out backward substitution */
      *(d + n - 1) = *(d + n - 1) / *(b + n - 1);
      for (i = n - 2; i >= 0; i--)
	*(d + i) = (*(d + i) - *(c + i) * *(d + i + 1)) / *(b + i);
      return true;
    }
  else
    return false;
}


