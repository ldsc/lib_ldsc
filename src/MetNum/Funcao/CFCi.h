#ifndef CFCi_h
#define CFCi_h

#include <Base/CFuncao.h>

/**
 * @brief Representa função Ci.
 * @author André Duarte Bueno
 * */
class CFCi : public CFuncao
{

/// Função Ci, calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual double Go (double x = 0, double y = 0);	
};
#endif
