#ifndef CFErf_h
#define CFErf_h

#include "Base/CFuncao.h"

/**
 * @brief Representa função de erro Erf.
 * @author André Duarte Bueno
 * */
class CFErf:public CFuncao
{

/// Função Erf, calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual double Go (double x = 0, double y = 0);
};
#endif
