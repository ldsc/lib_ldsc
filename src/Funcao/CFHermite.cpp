
#include <Funcao/CFHermite.h>

void
CFHermite::CalculaL1 ()
{
  L1 = 2 * x;
}

void
CFHermite::CalculaL2 ()
{
  L2 = 2 * x * L1 - 2 * i * L0;
}

/*
// double Hermite(double x, int n)
double CFHermite::Go(double x,double y)
{
  int n=(int)y;
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
*/
