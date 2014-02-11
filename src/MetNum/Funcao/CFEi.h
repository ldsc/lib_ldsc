#ifndef CFEi_h
#define CFEi_h

#include "Base/CFuncao.h"

/**
 * @brief Representa função Ei.
 * @author André Duarte Bueno
 * */
class CFEi:public CFuncao
{
/// Função Ei, calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual double Go (double x = 0, double y = 0);
};
#endif
