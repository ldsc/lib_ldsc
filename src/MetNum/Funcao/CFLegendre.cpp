

#include <MetNum/Funcao/CFLegendre.h>

void
CFLegendre::CalculaL1 ()
{
  L1 = x;
}

void
CFLegendre::CalculaL2 ()
{
  L2 = ((2 * i + 1) * L1 - i * L0) / (i + 1);
}

/*
// double Legendre(double x, double n)
double CFLegendre::Go(double x,double y)
{
  int n=(int)y;
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
*/
