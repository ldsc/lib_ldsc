#ifndef CFBesselJ_h
#define CFBesselJ_h

#include <Base/CFuncao.h>

/**
 * @brief Representa função de BesselJ.
 * @author André Duarte Bueno
 * */
class CFBesselJ : public CFuncao
{
/// Polinômio de Bessel, calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual double Go (double x = 0, double y = 0);	

  // double BesselJ(double z, int v);
};
#endif
