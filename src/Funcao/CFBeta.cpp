
// objeto utilizado
#include <Funcao/CFGamma.h>

#include <Funcao/CFBeta.h>

// double beta(double z, double w)
double
CFBeta::Go (double x, double y)
{
  // double z=x;

  // double w=y;

  int v = (int) y;

  CFGamma gamma;		// Cria objeto CFGamma

  return gamma (x) * gamma (y) / gamma (x + y);	// Calcula Beta usando a função gamma
}

/*double beta(double z, double w)
{
  return gamma(z) * gamma(w) / gamma(z + w);
}*/
