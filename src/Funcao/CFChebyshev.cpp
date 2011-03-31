

#include <Funcao/CFChebyshev.h>

void
CFChebyshev::CalculaL1 ()
{
  L1 = x;
}

void
CFChebyshev::CalculaL2 ()
{
  L2 = 2 * x * L1 - 2 * L0;
}

/*
// double Chebyshev(double x, int n)
double CFChebyshev::Go(double x,double y)
{
  int n=(int)y;
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
*/
