#ifndef CFBeta_h
#define CFBeta_h

#include <Base/CFuncao.h>

/**
 * @brief Representa função Beta.
 * @author André Duarte Bueno
 * */
class CFBeta:public CFuncao
{

/// Função Beta, calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual double Go (double x = 0, double y = 0);	// calcula a funcao em si y=f(x) ou z=f(x,y)

};
#endif
