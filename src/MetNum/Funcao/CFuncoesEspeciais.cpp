
#include <cmath>
using namespace std;
// #include <Estatistica/CEFatorial.h>

#include <MetNum/Funcao/CFuncoesEspeciais.h>

double
CFuncoesEspeciais::gamma (double x)
{
  double c[27] = { 0.0, 1.000000000, 0.5772156649, -0.6558780715,
    -4.2002635034e-2, 0.16665386113, -4.21977345555e-2, -9.6219715278e-3,
    7.21894324666e-3, -1.1651675918591e-3, -2.152416741149e-4,
    1.280502823882e-4,
    -2.01348547807e-5, -1.2504934821e-6, 1.1330272320e-6, -2.056338417e-7,
    6.1160950e-9, 5.0020075e-9, -1.1812746e-9, 1.043427e-10,
    7.7823e-12, -3.6968e-12, 5.1e-13, -2.06e-14, -5.4e-15, 1.4e-16, 1e-16
  };

  double sum = 0;

  int i;

  for (i = 26; i > 0; i--)
    sum = (sum + c[i]) * x;

  return 1 / sum;
}

double
CFuncoesEspeciais::Beta (double z, double w)
{
  return gamma (z) * gamma (w) / gamma (z + w);
}

double
CFuncoesEspeciais::erf (double x)
{
  double t = 1 / (1 + 0.32759 * x);

  double a[6] = { 0, 0.254829592, -0.284496736,
    1.424143741, -1.453152027, 1.061405429
  };

  double sum = 0;

  int i;

  for (i = 5; i > 0; i--)
    sum = (sum + a[i]) * t;

  return 1 - sum / exp (x * x);
}

double
CFuncoesEspeciais::Ei (double x)
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

// usa fatorial
double
CFuncoesEspeciais::Si (double x)
{
  const double tolerance = 1.e-8;

  double sum = 0;

  double term;

  double pow = x;

  double chs = 1;

  int n = 0;

  do
    {
      term = chs * pow / (2 * n + 1) / factorial (2 * n + 1);
      sum += term;
      /* update components of the next term */
      n++;
      chs = -chs;
      pow *= x * x;
    }
  while (fabs (term) > tolerance);

  return sum;
}

// usa fatorial
double
CFuncoesEspeciais::Ci (double x)
{
  const double tolerance = 1.e-8;

  double sum = 0;

  double term;

  double pow = x * x;

  double chs = -1;

  int n = 1;

  do
    {
      term = chs * pow / (2 * n) / factorial (2 * n);
      sum += term;

      /* update components of the next term */
      n++;

      chs = -chs;

      pow *= x * x;

    }
  while (fabs (term) > tolerance);

  return 0.5772156649 + log (x) + sum;
}

double
CFuncoesEspeciais::Laguerre (double x, int n)
{
  double L0 = 1;

  double L1 = 1 - x;

  double L2;

  int i = 2;

  if (n < 0)
    return -1;

  if (n == 0)
    return L0;
  else if (n == 1)
    return L1;
  else
    {
      while (i < n)
	{
	  L2 = (1 + 2 * i - x) * L1 - i * i * L0;
	  L0 = L1;
	  L1 = L2;
	  i++;
	}
      return L2;
    }
}

double
CFuncoesEspeciais::Hermite (double x, int n)
{
  double H0 = 1;

  double H1 = 2 * x;

  double H2;

  int i = 2;

  if (n < 0)
    return -1;

  if (n == 0)
    return H0;
  else if (n == 1)
    return H1;
  else
    {
      while (i < n)
	{
	  H2 = 2 * x * H1 - 2 * i * H0;

	  H0 = H1;

	  H1 = H2;

	  i++;
	}
      return H2;
    }
}

double
CFuncoesEspeciais::Chebyshev (double x, int n)
{
  double T0 = 1;

  double T1 = x;

  double T2;

  int i = 2;

  if (n < 0)
    return -1;

  if (n == 0)
    return T0;
  else if (n == 1)
    return T1;
  else
    {
      while (i < n)
	{
	  T2 = 2 * x * T1 - 2 * T0;
	  T0 = T1;
	  T1 = T2;
	  i++;
	}
      return T2;
    }
}

double
CFuncoesEspeciais::Legendre (double x, double n)
{
  double L0 = 1;

  double L1 = x;

  double L2;

  int i = 2;

  if (n < 0)
    return -1;

  if (n == 0)
    return L0;
  else if (n == 1)
    return L1;
  else
    {
      while (i < n)
	{
	  L2 = ((2 * i + 1) * L1 - i * L0) / (i + 1);
	  L0 = L1;
	  L1 = L2;
	  i++;
	}
      return L2;
    }
}

// usa fatorial e gamma
double
CFuncoesEspeciais::BesselJ (double z, int v)
{
  const double tolerance = 1.0e-8;

  double sum = 0;

  double term;

  int i = 0;

  do
    {
      term = pow (-z * z / 4, (double) i) / factorial (i) / gamma (v + i + 1);
      i++;
    }
  while (fabs (term) > tolerance);

  return pow (z / 2, v) * sum;
}

// usa BesselJ
double
CFuncoesEspeciais::BesselY (double z, int v)
{
  double pi = 4 * atan (1);

  return (BesselJ (z, v) * cos (v * pi) - BesselJ (z, -v)) / sin (v * pi);
}
