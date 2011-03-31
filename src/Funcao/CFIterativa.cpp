

#include <Funcao/CFIterativa.h>

double
CFIterativa::Go (double _x, double _y)
{
  this->x = _x;
  int n = (int) _y;
  L0 = 1;
  CalculaL1 ();			// funcao virtual da classe
  i = 2;

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
	  CalculaL2 ();		// funcao virtual da classe
	  L0 = L1;
	  L1 = L2;
	  i++;
	}
      return L2;
    }
}
