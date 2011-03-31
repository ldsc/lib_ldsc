


#include <cmath>

#include <Funcao/CFBesselJ.h>

#include <Funcao/CFBesselY.h>

// double BesselY(double z, int v)
double
CFBesselY::Go (double x, double y)
{
  // double z=x;                                                         // trocar uso de z por x

  int v = (int) y;		// converte para inteiro

  CFBesselJ besselJ;		// Cria objeto CFBesselJ
  // double pi = 4 * atan(1);                            // usar M_PI da math.h

  return (besselJ (x, v) * cos (v * M_PI) - besselJ (x, -v)) / sin (v * M_PI);
}

// usa BesselJ
/*double BesselY(double z, int v)
{
  double pi = 4 * atan(1);

  return (BesselJ(z, v) * cos(v * pi) - BesselJ(z, -v))
			 / sin(v * pi);
}       */
