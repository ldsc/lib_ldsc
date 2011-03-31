// Funcoes com dois parametros

#include <Funcao/CFLaguerre.h>

void
CFLaguerre::CalculaL1 ()
{
  L1 = 1 - x;
}

void
CFLaguerre::CalculaL2 ()
{
  L2 = (1 + 2 * i - x) * L1 - i * i * L0;
}

/*
// double Laguerre(double x, int n)
double CFLaguerre::Go(double x,double y)
{
  int n=(int)y;
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
*/
