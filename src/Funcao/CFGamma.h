#ifndef CFGamma_h
#define CFGamma_h

#include "Base/CFuncao.h"

// Funcoes com 1 parametro
/**
 * @brief Representa função Gamma.
 * @author André Duarte Bueno
 * */
class CFGamma : public CFuncao
{

// Métodos
public:
	
/// Função Gamma, calcula a funcao em si y=f(x) ou z=f(x,y)	
  virtual double Go (double x = 0, double y = 0);
  
};

#endif
