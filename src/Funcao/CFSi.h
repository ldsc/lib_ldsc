#ifndef CFSi_h
#define CFSi_h

#include <Base/CFuncao.h>

/**
 * @brief Representa função de Bessel.
 * @author André Duarte Bueno
 * */
class CFSi : public CFuncao
{
	
/// Calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual double Go (double x = 0, double y = 0);	
};
#endif
